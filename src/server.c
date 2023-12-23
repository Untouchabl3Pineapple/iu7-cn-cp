#include "server.h"

void start_server(void) {
    int server_socket;
    struct sockaddr_in server_addr;

    // Создание сокета
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Установка параметров сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязка сокета
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Прослушивание входящих подключений
    if (listen(server_socket, SOMAXCONN) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    log_message(LOG_INFO, "Server listening on port %d...\n", PORT);

    // Создание дочерних процессов (prefork)
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        pid_t pid = fork();

        if (pid == 0) { // Дочерний процесс
            int client_sockets[MAX_CLIENTS] = { 0 };
            int max_fd, activity, addr_len, client_socket;
            struct sockaddr_in client_addr;
            fd_set readfds;

            while (1) {
                FD_ZERO(&readfds); // набор фд пустой
                FD_SET(server_socket, &readfds); // добавление серверного сокета в набор

                max_fd = server_socket;

                for (int j = 0; j < MAX_CLIENTS; ++j) {
                    if (client_sockets[j] > 0) {
                        FD_SET(client_sockets[j], &readfds); // добавление клиентских сокетов в набор
                        if (client_sockets[j] > max_fd) {
                            max_fd = client_sockets[j];
                        }
                    }
                }

                // Использование select() для обработки нескольких соединений
                activity = select(max_fd + 1, &readfds, NULL, NULL, NULL); // блокировка + вовзрат кол-ва дескрипторов, на которых произошли события
                if ((activity < 0) && (errno != EINTR)) {
                    perror("Select error");
                    exit(EXIT_FAILURE);
                }

                // Проверка наличия входящего подключения
                if (FD_ISSET(server_socket, &readfds)) {
                    addr_len = sizeof(client_addr);
                    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addr_len)) == -1) {
                        perror("Accept failed");
                        continue;
                    }

                    // printf("New connection, socket fd is %d, ip is : %s, port : %d\n", client_socket,
                    //     inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                    // Добавление нового клиентского сокета в массив
                    for (int j = 0; j < MAX_CLIENTS; ++j) {
                        if (client_sockets[j] == 0) {
                            client_sockets[j] = client_socket;
                            break;
                        }
                    }
                }

                // Проверка данных от клиентов
                for (int j = 0; j < MAX_CLIENTS; ++j) {
                    if (client_sockets[j] > 0 && FD_ISSET(client_sockets[j], &readfds)) {
                        handle_client(client_sockets[j]);
                        close(client_sockets[j]);
                        client_sockets[j] = 0;
                    }
                }
            }
        }
        else if (pid > 0) {
            // Родительский процесс
        }
        else {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        wait(NULL);
    }
}

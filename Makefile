CC = gcc
CFLAGS = -Wall -Wextra

all: server

server: src/main.c
	$(CC) $(CFLAGS) src/main.c src/server.c src/http_handler.c src/logger.c -o build/server

clean:
	rm -f build/server

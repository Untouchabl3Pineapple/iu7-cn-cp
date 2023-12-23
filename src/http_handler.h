#ifndef __HTTP_HANDLER_H__
#define __HTTP_HANDLER_H__

#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "logger.h"

#define FORBIDDEN               403
#define NOT_FOUND               404
#define METHOD_NOT_ALLOWED      405

#define STATIC_ROOT             "/Users/ilya/cn-cg/static"

void send_directory_listing(int client_socket, const char* current_path, const char* directory_path);
void handle_client(int client_socket);
void send_file(int client_socket, const char* file_path, const char* method);
void send_error(int client_socket, int status_code);

#endif // __HTTP_HANDLER_H__

#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#include "http_handler.h"
#include "logger.h"

#define PORT            5600
#define MAX_CLIENTS     1000
#define NUM_CHILDREN    5

void start_server();

#endif // __SERVER_H__

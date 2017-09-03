#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 5050
#define SERVER_IP_ADDR "127.0.0.1"
#define BUF_SIZE 256
#define UUID_SIZE_FOR_STR 32
#define FILE_BUF_SIZE 1024

char* client_send_json_file_and_get_uuid(char* filename);

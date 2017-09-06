
#ifndef C_SERVERS_SERVER_H
#define C_SERVERS_SERVER_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>

int run_server(int argc, char **argv);

#endif //C_SERVERS_SERVER_H

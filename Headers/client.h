#ifndef C_SERVERS_CLIENT_H
#define C_SERVERS_CLIENT_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

int run_client(int argc, char **argv);

#endif //C_SERVERS_CLIENT_H

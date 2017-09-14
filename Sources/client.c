#include "../Headers/client.h"

int run_client(int argc, char **argv){

    // Revisar que exista el host y el puerto
    if(argc < 2) {
        printf("Modo de uso: ./C_Servers -c <host> <puerto> <solicitud>]");
        exit(EXIT_SUCCESS);
    }

    // Obteniendo argumentos
    char * host = argv[0];
    char * port = argv[1];

    // TODO Si son varios archivos entonces se separan con comas
    char * request = argv[2];

    // Conectando al servidor
    addrinfo * host_info = get_host_info(host, port);
    int client_fd = get_client_fd(host_info);
    connect_client(host_info, client_fd);

    // Ya conectado la infomación no se necesita
    freeaddrinfo(host_info);

    // Ya se pueden realizar solicitudes al servidor
    printf("Conectado con %s:%s\n", host_info->ai_canonname, port);

    // Realizar petición y recibir resultado
    char buffer[BUFFER_SIZE];

    GET(client_fd, request);
    while(recv(client_fd, buffer, BUFFER_SIZE, 0) > 0){
        fputs(buffer, stdout);
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(client_fd);

}

addrinfo * get_host_info(char * host, char * port){

    printf("Obteniendo información del host \n");

    addrinfo hints, * info;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(host, port, &hints, &info) != 0){
        perror("Error: get_host_info(2) \n");
        exit(EXIT_FAILURE);
    }

    return info;
}

int get_client_fd(addrinfo * host_info){

    printf("Obteniendo archivo descriptor del cliente \n");

    int family = host_info->ai_family;
    int socktype = host_info->ai_socktype;
    int protocol = host_info->ai_protocol;
    int client_fd = socket(family, socktype, protocol);

    if(client_fd < 0){
        perror("Error: get_client_fd(1) \n");
        exit(EXIT_FAILURE);
    }

    return client_fd;
}

void connect_client(addrinfo * host_info, int client_fd){

    printf("Conectando con el host \n");

    sockaddr * addr = host_info->ai_addr;
    socklen_t socklen = host_info->ai_addrlen;

    if(connect(client_fd, addr, socklen) < 0) {
        perror("Error: connect_client(2) \n");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

}

void GET(int client_fd, char * path) {

    char req[1000] = {0};
    sprintf(req, "GET %s HTTP/1.0\r\n\r\n", path);
    send(client_fd, req, strlen(req), 0);

}
#include "../Headers/server.h"

int setup_server(int argc, char **argv){

    if(argc < 1) {
        perror("Modo de uso: ./C_Servers <modo> <puerto> ?<nthreads> \n");
        exit(EXIT_SUCCESS);
    }

    // Obteniendo archivo descriptor del servidor
    char * port = argv[0];
    addrinfo * info = get_info(port);
    int server_fd = get_fd(info);
    reuse_fd(server_fd);

    // Colocando socket en modo de escucha
    bind_listener(info, server_fd);
    set_listening_mode(server_fd);

    return server_fd;

}

addrinfo * get_info(char *port){

    printf("Obteniendo información de la dirección \n");

    addrinfo hints, * info;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(NULL, port, &hints, &info) != 0){
        perror("Error: get_info(1) \n");
        exit(EXIT_FAILURE);
    }

    return info;
}

int get_fd(addrinfo * info){

    printf("Obteniendo archivo descriptor \n");

    int family = info->ai_family;
    int socktype = info->ai_socktype;
    int protocol = info->ai_protocol;
    int server_fd = socket(family, socktype, protocol);

    if(server_fd < 0){
        perror("Error: get_fd(1)\n");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void reuse_fd(int server_fd){

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0){
        perror("Error: reuse_fd(1) \n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

}

void bind_listener(addrinfo *info, int server_fd){

    printf("Asociando archivo descriptor con la información de la dirección \n");

    sockaddr * addr = info->ai_addr;
    socklen_t socklen = info->ai_addrlen;

    if(bind(server_fd, addr, socklen) < 0){
        perror("Error: bind_listener(2) \n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

}

void set_listening_mode(int server_fd){

    printf("Colocando en modo escucha \n");

    int max_requests = 15;
    if(listen(server_fd, max_requests) < 0){
        perror("Error: set_listening_mode(1) \n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

}
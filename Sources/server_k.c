#include "../Headers/server_f.h"

#define BUFFER_SIZE 1000

int run_server_k(int argc, char **argv){

    printf("Configurando servidor \n");
    int server = setup_server(argc, argv);
    printf("Servidor iniciado \n");

    socklen_t socklen;
    sockaddr_storage * sockaddr_s;

    for(int n_client = 0; 1 ; ++n_client){

        printf("Esperando cliente #%d \n", n_client);

        socklen = sizeof(sockaddr_s);
        int client = accept(server, (sockaddr *)&client, &socklen);
        if(client < 0){
            printf("Error aceptar al cliente #%d \n", n_client);
        }

        attend_request_k((void *)(long)client);
        printf("Cliente #%d conectado \n", n_client);

    }

}

void * attend_request_k(void * client_fd){

    char buffer[BUFFER_SIZE];
    int client = (int)(long) client_fd;

    // Se espera que lo primero que haga el cliente es usar el método GET
    recv(client, buffer, BUFFER_SIZE, 0);
    char * method = strtok(buffer, " ");
    if (strcmp(method, "GET") != 0)
        sprintf(buffer, "HTTP/1.0 Solo puedes usar el método GET\r\n\r\n");

    // Si el primer carácter es un '/' simplemente se ignora
    char * filename = strtok(NULL, " ");
    if (filename[0] == '/') filename++;

    // Revisa que el archivo exista
    if (access(filename, F_OK) != 0) {
        sprintf(buffer, "HTTP/1.0 403 El archivo %s no existe\r\n\r\n", filename);
    }

    // Revisa que el archivo tenga permisos de lectura
    if (access(filename, R_OK) != 0) {
        sprintf(buffer, "HTTP/1.0 403 Permiso denegado\r\n\r\n");
    }

    else {
        sprintf(buffer, "HTTP/1.0 200 Solicitud aceptada\r\n\r\n");
    }

    send(client, buffer, strlen(buffer), 0);

    FILE * file = fopen(filename, "r");
    while(fgets(buffer, BUFFER_SIZE, file)) {
        send(client, buffer, strlen(buffer), 0);
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(client);

}
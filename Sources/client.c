#include "../Headers/client.h"

int run_client(int argc, char **argv){

    // Revisar que exista el host y el puerto
    if(argc != 2) {
        perror("Modo de uso: ./C_Servers -c <host> <puerto>");
    }

    // Identificando el host y el puerto
    hostent * host = gethostbyname(argv[0]);
    int port = atoi(argv[1]);

    // Comprobación del host
    if(host < 0) {
        perror("El host ingresado es inválido \n");
        exit(EXIT_FAILURE);
    }

    // Estructura con la configuración del cliente
    sockaddr_in client_info;

    // Configuración del cliente
    memset(&client_info, 0, sizeof(client_info));
    client_info.sin_family = AF_INET;
    client_info.sin_port = htons((uint16_t) port);

    // Creación del socket
    int client_socket = socket(client_info.sin_family, SOCK_STREAM, 0);
    if(client_socket < 0){
        perror("Error en la apertura del socket \n");
        exit(EXIT_FAILURE);
    }

    // Conectando con el host
    int connection = connect(client_socket, (sockaddr *)&client_info, sizeof(client_info));
    if(connection < 0) {
        printf("Error al intentar conectar con el host \n");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Ya se pueden realizar solicitudes al servidor
    printf("Conectado con %s:%d\n", inet_ntoa(client_info.sin_addr), port);


}
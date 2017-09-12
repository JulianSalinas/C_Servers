#include "../Headers/server.h"


int setup_server(int argc, char **argv){

    if(argc < 1) {
        perror("Modo de uso: ./C_Servers <modo> <puerto> ?<nthreads>");
        exit(EXIT_SUCCESS);
    }

    // Identificando puerto
    int port = atoi(argv[0]);

    // Configuración del servidor
    sockaddr_in server_info;
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons((uint16_t) port);
    server_info.sin_addr.s_addr = INADDR_ANY;

    // Creación del socket
    int server_socket = socket(server_info.sin_family, SOCK_STREAM, 0);
    if(server_socket < 0)
        socket_error(server_socket, "Error en la apertura del socket \n");

    // Permite reusar el puerto si todavia no ha sido limpiado
    int set = 1;
    int sockopt = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
    if (sockopt < 0)
        socket_error(server_socket, "Error al reciclar el socket \n");

    // Asociar puerto a la conexión
    int n_bind = bind(server_socket, (sockaddr *)&server_info, sizeof(server_info));
    if(n_bind < 0)
        socket_error(server_socket, "Error al asociar el puerto a la conexión \n");

    // Establecer el socket en modo de escucha
    int n_listen = listen(server_socket, 10);
    if(n_listen < 0)
        socket_error(server_socket, "Error al colocar el socket en modo de escucha \n");

    return server_socket;

}

void socket_error(int socket, char * msg){
    perror(msg);
    close(socket);
    exit(EXIT_FAILURE);
}
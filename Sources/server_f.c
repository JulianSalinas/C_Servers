#include "../Headers/server_f.h"

int run_server_f(int argc, char **argv){

    int server = setup_server(argc, argv);

    // Ya se ha pueden aceptar conexiones
    printf("Servidor FIFO iniciado \n");

    sockaddr_in client_info;

    // Bucle infinito para atender a los clientes
    for(int n_client = 0; 1 ; ++n_client){

        printf("Esperando cliente #%d \n", n_client);

        socklen_t client_size = sizeof(client_info);
        int client = accept(server, (sockaddr *)&client_info, &client_size);
        if(client < 0){
            printf("Error aceptar al cliente #%d \n", n_client);
        }

        printf("Cliente #%d conectado \n", n_client);

    }

}

void * attend_request_f(void * client_socket){
    return 0;
}
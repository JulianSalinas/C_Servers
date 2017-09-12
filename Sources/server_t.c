#include "../Headers/server_t.h"

void * attend_request_t(void * client_socket){

    int client = (int)(long) client_socket;

    char * msg = "Eres el cliente #%s \n";
    sprintf(msg, msg, client);
    send(client, msg,  26, 0);

//    if(recv(client, buffer, 100, 0) < 0) {
//        printf("Error al recibir los datos\n");
//    }
//    else {
//
//        printf("%s\n", buffer);
//        bzero((char *)&buffer, sizeof(buffer));
//        send(conexion_cliente, "Recibido\n", 13, 0);
//    }

    //write(client, "hola2", 7);
    close(client);

    return 0;

}

int run_server_t(int argc, char **argv){

    // args[0] = tipo de servidor
    // args[1] = puerto

    int port = atoi(argv[1]);
    printf("Iniciando servidor en el puerto: %d \n", port);

    // Estructuras que contendrán la configuración a usar
    sockaddr_in server_info;
    sockaddr_in client_info;

    // Configuración del servidor
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons((uint16_t) port);
    server_info.sin_addr.s_addr = INADDR_ANY;

    // Creación del socket
    int server_socket = socket(server_info.sin_family, SOCK_STREAM, 0);
    if(server_socket < 0){
        printf("Error en la apertura del socket \n");
        exit(-1);
    }

    // Asociar puerto a la conexión
    int n_bind = bind(server_socket, (struct sockaddr *)&server_info, sizeof(server_info));
    if(n_bind < 0) {
        printf("Error al asociar el puerto a la conexión \n");
        exit(-1);
    }

    // Establecer el socket en modo de escucha
    int n_listen = listen(server_socket, 5);
    if(n_listen < 0){
        printf("Error al colocar el socket en modo de escucha \n");
        exit(-1);
    }

    // Aceptar conexiones
    int n_client = 0;
    int infinite_loop = 1;
    while(infinite_loop){

        socklen_t client_size = sizeof(client_info);
        int client_socket = accept(server_socket, (sockaddr *)&client_info, &client_size);
        if(client_socket < 0){
            printf("Error aceptar al cliente #%d \n", n_client);
            exit(-1);
        }

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, attend_request, (void *)(long) client_socket);

    }

}
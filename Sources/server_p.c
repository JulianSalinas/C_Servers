#include "../Headers/server_p.h"

void * attendClients(void *arg);

List * clients;
pthread_mutex_t clientMutex = PTHREAD_MUTEX_INITIALIZER;

int run_server_p(int argc, char **argv){

    clients = new_list();

    long threadAmount = strtol(argv[1], 0, 10);                     // Obtiene el param de numero de hilos

    int server = setup_server(argc, argv);

    for(int tCount = 0; tCount < threadAmount; ++tCount){
        pthread_t thread;
        if (pthread_create(&thread, 0, attendClients, NULL) < 0) {  // Inicia todos los hilos
            printf("\nError de inicializacion de hilos.\n");
        }
    }

    for(int n_client = 0; 1 ; ++n_client){
        client_info * cl = accept_client(server, n_client);         // Ciclo de aceptacion de clientes
        add(clients, cl);
    }

}

void * attendClients(void *arg){

    pthread_t id = pthread_self();
    pthread_detach(id);

    client_info * cl;
    printf("\nInicializado el hilo #%ld.\n", (long) id);

    for(int i=0; 1; ++i){                                           // Ciclo de atencion a clientes

        pthread_mutex_lock(&clientMutex);                           // Bloqueo de lista de clientes en espera

        if(peek(clients) != 0) cl =  (client_info *) pop(clients);  // Obtiene el primero de lista de espera
        else cl = NULL;

        pthread_mutex_unlock(&clientMutex);

        if (cl != NULL) {                                           // Si se encontro cliente, se atiende
            printf("\nHilo #%ld atendiendo al cliente #%d.\n", (long) id, cl->client_id);
            attend(cl);
            free(cl);
        }

    }
}
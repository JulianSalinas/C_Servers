#include "../Headers/server_p.h"

void * attendClients(void *arg);

List * clients;
pthread_mutex_t clientMutex = PTHREAD_MUTEX_INITIALIZER;

int run_server_p(int argc, char **argv){

    clients = new_list();

    int threadAmount = 3;

    int server = setup_server(argc, argv);

    for(int tCount = 0; tCount < threadAmount; ++tCount){
        pthread_t thread;
        if (pthread_create(&thread, 0, attendClients, NULL) < 0) {
            printf("Error de inicializacion de hilos.\n");
        }
    }

    for(int n_client = 0; 1 ; ++n_client){

        client_info * cl = accept_client(server, n_client);
        add(clients, cl);
    }

}

void * attendClients(void *arg){

    pthread_t id = pthread_self();
    pthread_detach(id);
    client_info * cl;

    printf("Inicializado el hilo #%ld.\n", (long) id);

    for(int i=0; 1; ++i){
        pthread_mutex_lock(&clientMutex);
        if(peek(clients) != 0) {
            cl =  (client_info *) pop(clients);
        } else {
            cl = NULL;
        }
        pthread_mutex_unlock(&clientMutex);

        if (cl != NULL) {
            printf("Hilo #%ld atendiendo al cliente #%d.\n", (long) id, cl->client_id);
            attend(cl);
            free(cl);
        } else {
            printf("Hilo #%ld en SLEEP.\n", (long) id);
        }
        sleep(5);
    }
}
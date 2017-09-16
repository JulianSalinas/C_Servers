#include "../Headers/server_p.h"

void * run_thread2(void * arg);
void * thread_managing(void * arg);

List * availableThreads;
List * busyThreads;
List * acceptedClients;

int run_server_p(int argc, char **argv){

    availableThreads = new_list();
    busyThreads = new_list();
    acceptedClients = new_list();

    int threadAmount = 3;

    int server = setup_server(argc, argv);

    for(int tCount = 0; tCount < threadAmount; ++tCount){
        pthread_t thread;
        add(availableThreads, &thread);
    }

    pthread_t threadManager;

    if(pthread_create(&threadManager, 0, thread_managing, NULL) < 0 ){
        printf("Error en el inicio del servidor.\n");
    }

    for(int n_client = 0; 1 ; ++n_client){

        client_info * cl = accept_client(server, n_client);
        add(acceptedClients, cl);
        printf("aceptado: %d\n", n_client);
    }

}

void * thread_managing(void * arg){
    pthread_t id = pthread_self();
    pthread_detach(id);

    for(int i = 0; 1; ++i){
        int x = 0;
    }

}

void * run_thread2(void * arg){

    pthread_t id = pthread_self();
    pthread_detach(id);

    client_info * cl =  (client_info *) arg;
    printf("Hilo %ld atendiendo al cliente #%d\n", (long) id, cl->client_id);

    attend(cl);

    free(cl);

    pthread_exit(0);

}
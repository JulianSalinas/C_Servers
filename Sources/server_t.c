#include "../Headers/server_t.h"

void * run_thread(void * arg);

int run_server_t(int argc, char **argv){

    int server = setup_server(argc, argv);

    for(int n_client = 0; 1 ; ++n_client){

        pthread_t thread;
        client_info * cl = accept_client(server, n_client);

        if(pthread_create(&thread, 0, run_thread, cl) < 0 )
            printf("No se ha podido atender al cliente #%d\n", n_client);

    }

}

void * run_thread(void * arg){

    pthread_t id = pthread_self();
    pthread_detach(id);

    client_info * cl =  (client_info *) arg;
    printf("Hilo %ld atendiendo al cliente #%d\n", (long) id, cl->client_id);

    attend(cl);

    free(cl);

    sleep(1);

    pthread_exit(0);

}
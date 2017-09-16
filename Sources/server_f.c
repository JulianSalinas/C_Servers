#include "../Headers/server_f.h"

int run_server_f(int argc, char **argv){

    int server = setup_server(argc, argv);

    for(int n_client = 0; 1 ; ++n_client){

        client_info * cl = accept_client(server, n_client);

        if(cl) {
            printf("Atendiendo al cliente #%d\n", n_client);
            attend((void *)cl);
        }
        else {
            printf("No se ha podido atender al cliente #%d\n", n_client);
        }

        free(cl);
    }

}
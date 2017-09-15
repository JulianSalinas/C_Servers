#include "../Headers/server_k.h"

int run_server_k(int argc, char **argv){

    int server = setup_server(argc, argv);
    for(int n_client = 0; 1 ; ++n_client){
        client_info * cl = accept_client(server, n_client);
        if(cl) attend((void *)cl);
        free(cl);
    }

}
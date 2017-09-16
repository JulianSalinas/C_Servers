#include "../Headers/server_k.h"

int run_server_k(int argc, char **argv){

    int server = setup_server(argc, argv);

    int processID = getpid();

    for(int n_client = 0; 1 ; ++n_client){

        processID = fork();             // Por cada solicitud se crea un proceso que la atienda
        // La funcion fork() retorna un entero != 0, por lo que el padre no entra al if()
        if (processID == 0){            // El nuevo proceso comienza con la var processID con valor 0
            client_info * cl = accept_client(server, n_client);
            printf("ID del proceso que atiende: %d\n\n", getpid());
            if(cl) attend((void *)cl);
            free(cl);
            kill(getpid(), SIGKILL);    // Finalizar el proceso hijo luego de atender la solicitud
        }

    }

}
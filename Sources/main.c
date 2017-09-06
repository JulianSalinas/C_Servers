#include "../Headers/main.h"

void * mythread(int * n_thread){

    printf("\nSoy el hilo %d", n_thread);
    return NULL;
}

int main(int argc, char *argv[])
{

    if(strcmp(argv[1], "-s") == 0){

        printf("Abriendo servidor");
        run_server(--argc, ++argv);

    }
    else{

        printf("Abriendo cliente");
        run_client(--argc, ++argv);

    }

//    for(int i = 0; i<argc; i++){
//
//        printf("Argumento %s \n", argv[i]);
//
//    }

    //test_list();



    getchar();
    return 0;

}
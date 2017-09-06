#include "../Headers/main.h"

void * mythread(int * n_thread){

    printf("\nSoy el hilo %d", n_thread);
    return NULL;
}

int main(int argc, char *argv[])
{

    for(int i = 0; i<argc; i++){

        printf("Argumento %s \n", argv[i]);

    }

    test_list();
    getchar();
    return 0;

}
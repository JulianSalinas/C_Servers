#include "../Headers/main.h"

/**
 * Modos de ejecución
 * -c : Cliente
 * -f : Servidor FIFO
 * -k : Servidor FORK
 * -t : Servidor THREAD
 * -p : Servidor PTHREAD
 */

int main(int argc, char *argv[]) {

    //Comprobar argumentos
    printf("Comprobando argumentos\n");
    for (int i = 0; i < argc; ++i) {
        printf("Argumento %d: %s \n", i, argv[i]);
    }

    // Eliminar nombre del programa de los argumentos
    --argc; ++argv;

    // Modo de ejecución (servidor o cliente)
    char * mode = argv[0];

    // Ejecutar según el modo
    // TODO Todos funcionan igual que el FIFO

    if(strcmp(mode, "-c") == 0)
        run_client(--argc, ++argv);

    else if (strcmp(mode, "-f") == 0)
        run_server_f(--argc, ++argv);

    else if (strcmp(mode, "-k") == 0)
        run_server_k(--argc, ++argv);

    else if (strcmp(mode, "-p") == 0)
        run_server_p(--argc, ++argv);

    else if (strcmp(mode, "-t") == 0)
        run_server_t(--argc, ++argv);

    else
        printf("Comando %s desconocido\n", mode);

    return 0;

}
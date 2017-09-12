#ifndef C_SERVERS_SERVER_K_H
#define C_SERVERS_SERVER_K_H

#include "server_a.h"

/**
 * Inicializa el servidor y luego atiende a los clientes
 */
int run_server_k(int, char **);

/**
 * Función para el thread que atiende las solicitudes del cliente
 */
void * attend_request_k(void *);

#endif //C_SERVERS_SERVER_K_H

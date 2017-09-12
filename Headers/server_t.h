#ifndef C_SERVERS_SERVER_H
#define C_SERVERS_SERVER_H

#include "server.h"

/**
 * Inicializa el servidor y luego atiende a los clientes
 */
int run_server_t(int, char **);

/**
 * Función para el thread que atiende las solicitudes del cliente
 */
void * attend_request_t(void *);

#endif //C_SERVERS_SERVER_H

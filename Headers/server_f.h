#ifndef C_SERVERS_SERVER_F_H
#define C_SERVERS_SERVER_F_H

#include "server.h"

/**
 * Inicializa el servidor y luego atiende a los clientes
 */
int run_server_f(int, char **);

/**
 * Función para el thread que atiende las solicitudes del cliente
 */
void * attend_request_f(void *);

#endif //C_SERVERS_SERVER_F_H

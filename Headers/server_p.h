#ifndef C_SERVERS_SERVER_P_H
#define C_SERVERS_SERVER_P_H

#include "server_a.h"

/**
 * Inicializa el servidor y luego atiende a los clientes
 */
int run_server_p(int, char **);

/**
 * Función para el thread que atiende las solicitudes del cliente
 */
void * attend_request_p(void *);

#endif //C_SERVERS_SERVER_P_H

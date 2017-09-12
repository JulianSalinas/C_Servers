#ifndef C_SERVERS_SERVER_H_H
#define C_SERVERS_SERVER_H_H

/**
 * Este archivo sirve para los 4 tipos de servidores
 */

#include "main.h"
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct hostent hostent;
typedef struct client client;

/**
 * Crea el socket del servidor con base al puerto y coloca en modo de escucha
 * Esta función es independiente del tipo de servidor
 * @return socket del servidor
 */
int setup_server(int, char **);

/**
 * Cierra el socket si ocurre un error, luego cierra el servidor.
 * Podría reiniciar el sistema (a futuro)
 */
void socket_error(int, char*);

#endif //C_SERVERS_SERVER_H_H

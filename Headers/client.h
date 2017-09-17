#ifndef C_SERVERS_CLIENT_H
#define C_SERVERS_CLIENT_H

#define BUFFER_SIZE 8192
#define MAX_FILENAME 256

#include "main.h"

typedef struct addrinfo addrinfo;

/**
 * Función principal del cliente. Esta es llamada desde el main únicamente
 * @param argc = 3
 * @param argv:
 *          argv[0] = Nombre o dirección del servidor
 *          argv[1] = Puerto donde escucha el servidor
 *          argv[2] = Solicitudes (nombres de archivos separados por comas)
 * @return
 */
int run_client(int argc, char **argv);

/**
 * Obtiene la información del servidor con base a su nombre o dirección
 * @param host: Nombre o dirección del servidor
 * @param port: Puerto donde el servidor está escuchando las solicitudes
 * @return Lista enlazada con las posibles configuraciones.
 * Solo se usa la primera.
 */
addrinfo * get_host_info(char * host, char * port);

/**
 * Obtiene el archivo descriptor del cliente (socket) con base a la información
 * del servidor con el que se quiere conectar.
 * @param host_info: Información del servidor a conectarse
 * @return archivo descriptor del cliente (socket)
 */
int get_client_fd(addrinfo * host_info);

/**
 * Conecta el cliente al servidor.
 * @param host_info: Información del servidor a conectarse
 * @param client_fd: archivo descriptor del cliente (socket)
 */
//void connect_client(addrinfo * host_info, int client_fd);

#endif //C_SERVERS_CLIENT_H

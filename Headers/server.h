#ifndef C_SERVERS_SERVER_H_H
#define C_SERVERS_SERVER_H_H

#define BUFFER_SIZE 1024

/**
 * Este archivo sirve para los 4 tipos de servidores
 */

#include "main.h"

typedef struct sockaddr_storage sockaddr_storage;
typedef struct sockaddr sockaddr;
typedef struct addrinfo addrinfo;

/**
 * Crea el socket del servidor con base al puerto y coloca en modo de escucha
 * Esta función es independiente del tipo de servidor
 * @return archivo descriptor del servidor (socket)
 */
int setup_server(int, char **);

/**
 * Obtiene una lista enlazada de todas las posibles configuraciones del
 * servidor. Solo la primera es usada.
 * @param port: puerto para escuchar solicitudes
 */
addrinfo * get_info(char * port);

/**
 * Obtiene el archivo descriptor del servidor.
 * @param info: Configuración obtenida con 'get_info(1)'
 * @return archivo descriptor del servidor (socket)
 */
int get_fd(addrinfo * info);

/**
 * Elimina el problema de reiniciar el servidor 30 segundos despues
 * @param server_fd: socket
 */
void reuse_fd(int server_fd);

/**
 * Solicita al SO poner el socket a la escucha
 * @param info: Configuración obtenida con 'get_info(1)'
 * @param server_fd: socket
 */
void bind_listener(addrinfo * info, int server_fd);

/**
 * El socket queda listo para resibir solicitudes.
 * A partir de aquí cada server atiene de forma diferente
 * @param server_fd: socket
 */
void set_listening_mode(int server_fd);


#endif //C_SERVERS_SERVER_H_H

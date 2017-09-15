#ifndef C_SERVERS_SERVER_H_H
#define C_SERVERS_SERVER_H_H

/**
 * Este archivo sirve para los 4 tipos de servidores
 */

#include "main.h"

// Abreviaciones de las estructuras
typedef struct sockaddr_storage sockaddr_storage;
typedef struct sockaddr sockaddr;
typedef struct addrinfo addrinfo;
typedef struct client_info client_info;

/**
 * Estructura para saber la información del cliente cuando se está
 * manejando la solicitud
 */
struct client_info{
    int client_id;
    int client_fd;
};

/**
 * Crea el socket del servidor con base al puerto y coloca en modo de escucha
 * Esta función es independiente del tipo de servidor
 * @return archivo descriptor del servidor (socket)
 */
int setup_server(int, char **);

/**
 * Función usada por los hilos o procesos para atender a los clientes
 * @param arg: Es una estructura client_info
 */
void * attend(void * arg);

/**
 * Obtiene una lista enlazada de todas las posibles configuraciones del
 * servidor. Solo la primera es usada
 * @param port: puerto para escuchar solicitudes
 */
addrinfo * get_info(char * port);

/**
 * Obtiene el archivo descriptor del servidor
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
 * El socket queda listo para recibir solicitudes
 * A partir de aquí cada server atiende de forma diferente
 * @param server_fd: socket
 */
void set_listening_mode(int server_fd);

/**
 * Acepta la conexión del cliente.
 * Toma el número de cliente y descriptor para retornarlo
 * @param server_fd: socket
 * @param n_client: número de cliente (dado en 'run_server')
 * @return estructura con n_client y client_fd
 */
client_info * accept_client(int server_fd, int n_client);

/**
 * Obtiene el nombre del archivo que el cliente solicita
 * @param client_fd: socket del cliente
 * @return nombre del archivo solicitado
 */
char * get_request_info(int client_fd);

/**
 * Envia un archivo abierto al cliente (aqui se asume que el archivo
 * fue leido correctamente pues fue validado antes de enviarlo)
 * @param client_fd: socket del cliente
 * @param file: archivo abierto y validado previamente
 * @return Indefinido XD
 */
int send_file(int client_fd, FILE * file);

#endif //C_SERVERS_SERVER_H_H

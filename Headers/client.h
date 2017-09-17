#ifndef C_SERVERS_CLIENT_H
#define C_SERVERS_CLIENT_H

#define BUFFER_SIZE 8192
#define MAX_FILENAME 256

#include "main.h"

typedef struct addrinfo addrinfo;
typedef struct request request;

/**
 * Estructura para pasar a la función perform_request(1), la cual,
 * es ejecutada por medio de un thread
 */
struct request{
    int client_fd;
    char * filename;
    char * storage_folder;
};

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
 * Solicita al servidor un archivo, luego recibe el archivo y
 * lo guardar, Al finalizar realiza una llamada al sistema para que
 * muestre el archivo recibido. Es ejecutada por la función
 * run_client(2) igual a la cantidad de archivos solicitados añ servidor
 * @param arg: estructura del tipo 'request'
 */
void * perform_request(void * arg);

/**
 * Función usada por perform_request(1)
 * Envia un solicitud con la forma GET filename HTTP/1.1\r\n\r\n
 * @param client_fd: socket por donde se envia la solicitud
 * @param filename: Nombre del archivo de la solicitud
 * @return != 0 si lo envió, == 0 sino
 */
int send_request(int client_fd, char * filename);

/**
 * Recibe el http response y lo guarda en un archivo temporal para
 * que posteriormente se pueda extraer la información requerida
 * del archivo de manera completa
 * @param from_fd: Descriptor de donde vienen los datos (en este caso el socket)
 * @param original_file_name: Nombre que se usará como prefijo para
 * nombrar al archivo temporal
 * @return Descriptor del nuevo archivo temporal creado
 */
int get_temporal_file(int from_fd, char * original_file_name);

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
 * del servidor con el que se quiere conectar. Si no puede conectarse, prueba
 * otra vez hasta que no existan configuraciones posibles
 * @param host_info: Información del servidor a conectarse
 * @return archivo descriptor del cliente (socket)
 */
int get_client_fd(addrinfo * host_info);

/**
 * Hace uso de las funciones get_host_info(2) y get_client_fd(1)
 * para establecer la conexión.
 * @param port: Puerto donde el servidor está escuchando las solicitudes
 * @return archivo descriptor del cliente (socket) conectado con el servidor
 */
int connect_client(addrinfo * host_info);

#endif //C_SERVERS_CLIENT_H

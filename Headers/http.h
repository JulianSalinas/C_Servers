#ifndef C_SERVERS_HTTP_H
#define C_SERVERS_HTTP_H

#include "main.h"

// Variables globales
#define BUFFER_SIZE 8192
#define MAX_FILENAME 256

// Códigos de OK http
#define HTTP_OK 200
#define HTTP_NOT_CONTENT 204

// Códigos de error http (deben coincidir con la carpeta Errors)
#define HTTP_BAD_REQUEST 400
#define HTTP_NOT_FOUND 404
#define HTTP_UNSUPPORTED_MEDIA_TYPE 415
#define HTTP_SERVER_ERROR 500

/**
 * Obtiene el encabezado del HTTP response.
 * Se tiene que enviar siempre antes de cualquier respuesta.
 * @param status: HTTP_OK, HTTP_BAD_REQUEST, HTTP_NOT_FOUND...ETC
 */
char * get_header(int status);

/**
 * Crea un string con base tamaño del archivo a enviar
 * Se asume que el archivo fue leido y validado correctamente
 * @param file: archivo abierto y validado previamente
 * @return "Content-Lenght: n \r\n"
 */
char * get_content_length(FILE * file);

/**
 * Crea un string con base a la extensión del archivo a enviar
 * Se asume que el archivo fue leido y validado correctamente
 * @param file: archivo abierto y validado previamente
 * @return "Content-Lenght: n \r\n"
 */
char * get_content_type(char * filename);

#endif //C_SERVERS_HTTP_H

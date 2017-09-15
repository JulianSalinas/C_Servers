#include "../Headers/server_f.h"

#define HTTP_OK 200
#define HTTP_BAD_REQUEST 400
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404

typedef struct client_info client_info;
struct client_info{
    int client_id;
    int client_fd;
};

typedef struct request_info request_info;
struct request_info{
    char * method;
    char * filename;
};

request_info * get_request_info(int client_fd){

    char buffer[BUFFER_SIZE] = {0};
    request_info * request = malloc(sizeof(request_info));

    // Se obtiene el método HTTP (posiblemente GET)
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Solicitud del browser: \n\n%s", buffer);

    if(strlen(buffer) == 0){
        request->filename = "../Templates/400.html";
    }
    else{
        request->method = strtok(buffer, " ");
        request->filename = strtok(0, " ");

        // Elimina el '/' del archivo
        if (request->filename[0] == '/')
            request->filename++;
    }

    return request;
}

void send_file(int client_fd, FILE * file){

    // Llena el buffer y lo envia (chunks)
    char buffer[BUFFER_SIZE] = {0};
    while(fgets(buffer, BUFFER_SIZE, file)) {
        write(client_fd, buffer, strlen(buffer));
        memset(buffer, 0, BUFFER_SIZE);
    }

}

void send_status(int client_fd, int status){

    // Se envia el statatus
    char response[50] = {0};
    if(status == HTTP_OK)
        sprintf(response, "HTTP/1.0 200 OK\r\n");
    else if(status == HTTP_BAD_REQUEST)
        sprintf(response, "HTTP/1.0 400 Bad Request\r\n");
    else if(status == HTTP_FORBIDDEN)
        sprintf(response, "HTTP/1.0 403 Forbidden\r\n");
    else if(status == HTTP_NOT_FOUND)
        sprintf(response, "HTTP/1.0 404 Not Found\r\n");
    write(client_fd, response, strlen(response));

}

void * attend_f(void * arg){

    int status = HTTP_OK;

    // Extrayendo información del cliente
    client_info * cl =  (client_info *) arg;
    int client_id = cl->client_id;
    int client_fd = cl->client_fd;

    // Extrayendo información de la solicitud
    request_info * request = get_request_info(client_fd);
    char * filename = request->filename;
    printf("El cliente #%d ha solicitado el archivo %s \n", client_id, filename);

    // Revisa que el archivo exista
    if (access(filename, F_OK)) status = HTTP_NOT_FOUND;

    // Revisa que el archivo tenga permisos de lectura
    else if (access(filename, R_OK)) status = HTTP_FORBIDDEN;

    send_status(client_fd, status);
    FILE * response_file;

    // Si el archivo cumplió las condicionaes, se abre dicho archivo
    if(status == HTTP_OK) response_file = fopen(filename, "r");

    else {

        // Sino, envia un template HTML con el error
        char response[50] = {0};
        sprintf(response, "../Templates/%d.html", status);
        response_file = fopen(response, "r");

    }

    /**
     * Resto del encabezado
     */
    fseek(response_file, 0, SEEK_END);
    long file_size = ftell(response_file);
    rewind(response_file);

    char content_length[50] = {0};
    sprintf(content_length, "Content-Length: %ld\r\n", file_size);
    write(client_fd, content_length, strlen(content_length));

    char extension[10] = {0};
    if(strstr(filename, ".txt"))
        strcpy(extension, "text/plain");
    else if (strstr(filename, ".html"))
        strcpy(extension, "text/html");
    else if (strstr(filename, ".png"))
        strcpy(extension, "image/png");
    else if (strstr(filename, ".jpg"))
        strcpy(extension, "image/jpg");

    char content_type[50] = {0};
    sprintf(content_type, "Content-Type: %s\r\n", extension);
    write(client_fd, content_type, strlen(content_type));

    char * line = "\r\n";
    write(client_fd, line, strlen(line));

    /**
     * Fin del resto del encabezado
     */

    // Se envia ya sea el html con el error o el archivo solicitado
    if(response_file) {
        send_file(client_fd, response_file);
        fclose(response_file);
    }

    close(client_fd);

}

int run_server_f(int argc, char **argv){

    printf("Configurando servidor \n");
    int server = setup_server(argc, argv);
    printf("Servidor iniciado \n");

    for(int n_client = 0; 1 ; ++n_client){

        sockaddr_storage * sockaddr_s;
        socklen_t socklen = sizeof(sockaddr_s);;
        client_info * cl = malloc(sizeof(client_info));
        cl->client_id = n_client;
        cl->client_fd = accept(server, (sockaddr *)&sockaddr_s, &socklen);

        if(cl->client_fd  < 0)
            printf("Error aceptar al cliente #%d \n", n_client);
        else
            attend_f((void *)cl);

        free(cl);
    }

}
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
    List * filenames;
};

request_info * get_request_info(int client_fd){

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    request_info * request = malloc(sizeof(request_info));

    // Se obtiene el método HTTP (posiblemente GET)
    recv(client_fd, buffer, BUFFER_SIZE, 0);
    printf("Solicitud del browser: \n\n%s", buffer);
    request->method = malloc(sizeof(char) * 15);
    strcpy(request->method, strtok(buffer, " "));

    // Se obtiene el string con las rutas de los archivos solicitados
    request->filenames = new_list();
    char * files_field = strtok(0, " ");

    // Se enlistan los archivos
    char * filename = strtok(files_field, ",");
    while(filename){
        if (filename[0] == '/') filename++;
        add(request->filenames, filename);
        filename = strtok(0, ",");
    }

    return request;
}

void send_file(int client_fd, FILE * file){

    char buffer[BUFFER_SIZE];
    while(fgets(buffer, BUFFER_SIZE, file)) {
        send(client_fd, buffer, strlen(buffer), 0);
        memset(buffer, 0, BUFFER_SIZE);
    }

}

void send_status(int client_fd, int status){

    // Se envia el statatus
    char response[50];
    if(status == HTTP_OK)
        sprintf(response, "HTTP/1.0 200 OK\r\n\r\n");
    else if(status == HTTP_BAD_REQUEST)
        sprintf(response, "HTTP/1.0 400 Bad Request\r\n\r\n");
    else if(status == HTTP_FORBIDDEN)
        sprintf(response, "HTTP/1.0 403 Forbidden\r\n\r\n");
    else if(status == HTTP_NOT_FOUND)
        sprintf(response, "HTTP/1.0 404 Not Found\r\n\r\n");
    send(client_fd, response, strlen(response), 0);

    // Se envia la plantilla html del error solo si existe
    memset(response, 0, strlen(response));
    sprintf(response, "../Templates/%d.html", status);
    FILE * response_file = fopen(response, "r");
    if(response_file) {
        send_file(client_fd, response_file);
        fclose(response_file);
    }

}

void * attend_f(void * arg){

    // Extrayendo información del cliente
    client_info * cl =  (client_info *) arg;
    int client_id = cl->client_id;
    int client_fd = cl->client_fd;
    free(cl);
    printf("Atendiendo al cliente #%d \n", client_id);

    request_info * request = get_request_info(client_fd);
    char * first_file = (char *)request->filenames->start->content;
    printf("Se ha solicitado el archivo %s \n", first_file);

    char * filename = first_file;

    // Revisa que el archivo exista
    if (access(filename, F_OK)) send_status(client_fd, HTTP_NOT_FOUND);

    // Revisa que el archivo tenga permisos de lectura
    if (access(filename, R_OK)) send_status(client_fd, HTTP_FORBIDDEN);

    else {
        send_status(client_fd, HTTP_OK);
        FILE * file = fopen(filename, "r");
        if(file) {
            send_file(client_fd, file);
            fclose(file);
        }
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
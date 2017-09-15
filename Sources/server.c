#include "../Headers/server.h"

int setup_server(int argc, char **argv){

    if(argc < 1) {
        perror("Modo de uso: ./C_Servers <modo> <puerto> ?<nthreads> \n");
        exit(EXIT_SUCCESS);
    }

    // Obteniendo archivo descriptor del servidor
    char * port = argv[0];
    addrinfo * info = get_info(port);
    int server_fd = get_fd(info);
    reuse_fd(server_fd);

    // Colocando socket en modo de escucha
    bind_listener(info, server_fd);
    set_listening_mode(server_fd);

    return server_fd;

}

void * attend(void * arg){

    // Variables usadas para enviar el archivo
    FILE * file = 0;
    int status = HTTP_OK;
    char * filename = 0;
    char * header = 0;
    char * content_type = 0;
    char * content_lenght = 0;

    // Extrayendo información del cliente
    client_info * cl =  (client_info *) arg;
    int client_id = cl->client_id;
    int client_fd = cl->client_fd;

    // Extrayendo información de la solicitud
    char * request = get_request_info(client_fd);

    // Si la solicitud esta vacia no se envia nada
    if(request == 0)
        status = HTTP_NOT_CONTENT;

        // Se revisa que el nombre del archivo sea válido (no contenga !@%^*~|)
    else if(!is_valid_filename(request))
        status = HTTP_BAD_REQUEST;

    else{

        printf("El cliente #%d ha solicitado el archivo %s \n", client_id, request);

        // Se mapea la solicitud al archivo en la carpeta Files del proyecto
        filename = malloc(FILENAME_MAX);
        strcpy(filename, "../Files/");
        strcat(filename, request);

        // Revisa que el archivo exista y que tenga permisos
        if (access(filename, F_OK) && access(filename, R_OK))
            status = HTTP_NOT_FOUND;

        else{

            // Revisa que la extensión del archivo sea HTML, TXT, PNG o JPG
            content_type = get_content_type(filename);

            if (content_type == 0)
                status = HTTP_UNSUPPORTED_MEDIA_TYPE;
        }

    }

    // Se envia el encabezado con el status
    header = get_header(status);
    write(client_fd, header, strlen(header));

    // Si sucedió un error entonces el archivo es un html con la razón
    if(status != HTTP_OK){
        char response[50] = {0};
        sprintf(response, "../Errors/%d.html", status);
        file = fopen(response, "r");
        content_lenght = get_content_length(file);
        content_type = get_content_type(filename);
        write(client_fd, content_type, strlen(content_type));
        write(client_fd, content_lenght, strlen(content_lenght));
    }

        // Sino, se envia el archivo solicitado
    else{
        file = fopen(filename, "r");
        content_lenght = get_content_length(file);
        write(client_fd, content_type, strlen(content_type));
        write(client_fd, content_lenght, strlen(content_lenght));
    }

    char * header_end = "\r\n";
    write(client_fd, header_end, strlen(header_end));

    send_file(client_fd, file);
    close(client_fd);

}

addrinfo * get_info(char *port){

    addrinfo hints, * info;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(NULL, port, &hints, &info) != 0){
        perror("Error: get_info(1) \n");
        exit(EXIT_FAILURE);
    }

    return info;
}

int get_fd(addrinfo * info){

    int family = info->ai_family;
    int socktype = info->ai_socktype;
    int protocol = info->ai_protocol;
    int server_fd = socket(family, socktype, protocol);

    if(server_fd < 0){
        perror("Error: get_fd(1)\n");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void reuse_fd(int server_fd){

    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0){
        perror("Error: reuse_fd(1) \n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

}

void bind_listener(addrinfo *info, int server_fd){

    sockaddr * addr = info->ai_addr;
    socklen_t socklen = info->ai_addrlen;

    if(bind(server_fd, addr, socklen) < 0){
        perror("Error: bind_listener(2) \n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

}

void set_listening_mode(int server_fd){

    int max_requests = BUFFER_SIZE;

    if(listen(server_fd, max_requests) < 0){
        perror("Error: set_listening_mode(1) \n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

}

client_info * accept_client(int server_fd, int n_client){

    sockaddr_storage * sockaddr_s;
    socklen_t socklen = sizeof(sockaddr_s);;
    client_info * cl = malloc(sizeof(client_info));

    cl->client_id = n_client;
    cl->client_fd = accept(server_fd, (sockaddr *)&sockaddr_s, &socklen);

    if(cl->client_fd  < 0){
        printf("Error aceptar al cliente #%d \n", n_client);
        return 0;
    }

    return cl;

}

char * get_request_info(int client_fd){

    char * filename = 0;
    char buffer[BUFFER_SIZE] = {0};
    read(client_fd, buffer, BUFFER_SIZE);

    if(strlen(buffer)) {
        strtok(buffer, " ");
        filename = strtok(0, " ");
        if (filename[0] == '/')
            filename++;
    }

    return filename;
}

int send_file(int client_fd, FILE * file){


    char buffer[BUFFER_SIZE];
    int len;
    int ret;

    for (ret = 0;;) {
        len = fread (buffer, 1, sizeof (buffer), file);
        if (len == 0) {
            ret = feof (file);
            break;
        }
        if (!send (client_fd, buffer, len, 0)) break;
    }

    fclose(file);


    return ret;

}
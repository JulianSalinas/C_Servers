#include <sys/stat.h>
#include <pwd.h>
#include "../Headers/client.h"

int run_client(int argc, char **argv){

    if(argc < 4) {
        printf("Modo de uso: ./C_Servers -c <host> <puerto>");
        printf("<ruta_guardado> <solicitud1,solitud2,solicitudN>\n");
        exit(EXIT_SUCCESS);
    }

    char * host = argv[0];
    char * port = argv[1];
    char * files = argv[3];
    char * storage_folder = map_folderpath_to_homedir(argv[2]);

    if(!storage_folder){
        printf("El nombre del folder especificado %s no es válido\n", storage_folder);
        exit(EXIT_FAILURE);
    }

    if(!mkdir_folder(storage_folder)){
        printf("No se ha podido crear el folder para guardar %s\n", storage_folder);
        exit(EXIT_FAILURE);
    }

    List * tokens = (List *) tokenizer(files, ",");
    List * threads = new_list();

    // Se crean todos los hilos para realizar las solicitudes
    Node * file_node = tokens->start;
    while(file_node != 0){

        pthread_t request_thread;

        addrinfo * host_info = get_host_info(host, port);
        request * req = malloc(sizeof(request));
        req->client_fd = connect_client(host_info);
        req->filename = file_node->content;
        req->storage_folder = storage_folder;

        if(pthread_create(&request_thread, 0, perform_request, req) < 0)
            printf("No se ha podido solicitar el archivo %s\n", req->filename);
        else add(threads, (void *)(long) request_thread);

        //perform_request(req);
        file_node = file_node->next;
    }

    Node * thread_node = threads->start;
    while(thread_node != 0){
        pthread_t pthread = (pthread_t)(long) thread_node->content;
        pthread_join(pthread, 0);
        thread_node = thread_node->next;
    }

    printf("Todos los archivos han sido recibidos\n");

}

void * perform_request(void * arg){

    request * req = (request *) arg;
    int client_fd = req->client_fd;
    char * filename = req->filename;
    char * storage_folder = req->storage_folder;

    char buffer[BUFFER_SIZE] = {0};
    memset(buffer, 0, BUFFER_SIZE);

    // Enviamos la solicitud con la forma "GET filename HTTP/1.1"
    if(send_request(req->client_fd, req->filename))
        printf("El archivo %s ha sido solicitado\n", req->filename);
    filename = get_absolute_filename(storage_folder, filename);

    // Creamos un archivo temporal con la información leída del socket.
    int tmp_file_fd = get_temporal_file(client_fd, filename);

    // Obtenemos el encabezado http presente en el archivo temporal
    if(!read(tmp_file_fd, buffer, BUFFER_SIZE)){
        printf("Error al obtener la respuesta a la solicitud\n");
        exit(EXIT_FAILURE);
    }

    // Obtenemos la primera linea, si es el status code 200 quiere decir
    // que recibimos el archivo con éxito
    char * status = extract_status_code(buffer);
    if(strcmp(status, "HTTP/1.1 200 OK") != 0){
        printf("El archivo %s no se ha obtenido", filename);
        printf(" por el siguiente error %s\n", status);
        exit(EXIT_FAILURE);
    }

    // Obtenemos el tamaño total del contenido sin contar el encabezado http
    // y colocamos el puntero donde debe empezar a leer el contenido
    size_t content_length = extract_content_lenght(buffer);
    lseek(tmp_file_fd, 0L, SEEK_END);
    size_t start_index = get_fd_size(tmp_file_fd) - content_length;
    lseek(tmp_file_fd, start_index, SEEK_SET);

    // Creamos el archivo y guardamos el contenido recibido
    FILE * file = fopen(filename, "w");
    copy_content(tmp_file_fd, fileno(file));
    fclose(file);

    // Cerramos el socket y mostramos el archivo resultante
    close(client_fd);
    //show_file(filename);

}

int send_request(int client_fd, char * filename){

    char * http_request = create_get_method(filename);
    return (int) write(client_fd, http_request, strlen(http_request));

}

int get_temporal_file(int from_fd, char * original_file_name){

    char * filename = get_temporal_filename(original_file_name);
    int tmp_file_fd = mkstemp(filename);
    copy_content(from_fd, tmp_file_fd);
    lseek(tmp_file_fd, 0, SEEK_SET);
    unlink(filename);
    return tmp_file_fd;

}

int connect_client(addrinfo * host_info){

    int client_fd = get_client_fd(host_info);

    sockaddr * addr = host_info->ai_addr;
    socklen_t socklen = host_info->ai_addrlen;

    if(connect(client_fd, addr, socklen) < 0) {
        perror("Error: connect_client(2) \n");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(host_info);
    return client_fd;

}

addrinfo * get_host_info(char * host, char * port){

    addrinfo hints, * info;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;

    if(getaddrinfo(host, port, &hints, &info) != 0){
        perror("Error: get_host_info(2) \n");
        exit(EXIT_FAILURE);
    }

    return info;
}

int get_client_fd(addrinfo * host_info){

    addrinfo * h = host_info;

    int client_fd = socket(h->ai_family, h->ai_socktype, h->ai_protocol);
    while(h->ai_next != 0 & client_fd < 0){
        h = h->ai_next;
        client_fd = socket(h->ai_family, h->ai_socktype, h->ai_protocol);
    }

    if(client_fd < 0){
        perror("Error: get_client_fd(1) \n");
        exit(EXIT_FAILURE);
    }

    return client_fd;
}
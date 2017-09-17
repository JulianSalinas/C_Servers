#include <sys/stat.h>
#include <pwd.h>
#include "../Headers/client.h"

int connect_client(char * host, char * port){

    addrinfo * host_info = get_host_info(host, port);
    int client_fd = get_client_fd(host_info);

    sockaddr * addr = host_info->ai_addr;
    socklen_t socklen = host_info->ai_addrlen;

    if(connect(client_fd, addr, socklen) < 0) {
        perror("Error: connect_client(2) \n");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(host_info);
    printf("Conectado con %s:%s\n", host, port);
    return client_fd;

}

char * get_storage_folder(char * incomplete_storage_folder){

    char * storage_folder = incomplete_storage_folder;

    if(!is_valid_folder(storage_folder))
        return 0;

    // Si la no especifica home, entonces es necesario colocarlo
    if(!strstr(storage_folder, "home/")){

        char * homedir = getenv("HOME");
        if(homedir == 0 ) homedir = getpwuid(getuid())->pw_dir;

        // Se concatena home con la folder que especificó el usuario
        char * tmp = storage_folder;
        storage_folder = malloc(MAX_FILENAME);
        strcpy(storage_folder, homedir);
        strcat(storage_folder, "/");
        strcat(storage_folder, tmp);
        strcat(storage_folder, "/");
    }

    return storage_folder;
}

int mkdir_folder(char * foldername){

    struct stat st = {0};
    if(stat(foldername, &st) < 0)
        return mkdir(foldername, 0700);
    return 1;

}

char * get_absolute_path(char * storage_folder, char * filename){

    char * absolute_path = malloc(FILENAME_MAX);
    strcpy(absolute_path, storage_folder);
    strcat(absolute_path, filename);
    return absolute_path;

}

char * get_tmp_path(char * absolute_path){

    char * tmp_path = malloc(FILENAME_MAX);
    strcat(tmp_path, absolute_path);
    strcat(tmp_path, ".tmp-XXXXXX");
    return tmp_path;

}

List * list_requests(char * filenames){

    List * requests = new_list();
    char * request = strtok(filenames, ",");
    while(request){
        add(requests, request);
        request = strtok(NULL,",");
    }
    return requests;

}


int run_client(int argc, char **argv){

    // Revisar que existan todos los argumentos
    if(argc < 4) {
        printf("Modo de uso: ./C_Servers -c <host> <puerto> <ruta_guardado> <solicitud1,solitud2;...solicitudN>");
        exit(EXIT_SUCCESS);
    }

    else{

        // Obteniendo argumentos
        char * host = argv[0];
        char * port = argv[1];
        char * files = argv[3];

        // Concatenamos homedir con el folder que el usuario quiere crear
        char * storage_folder = get_storage_folder(argv[2]);
        if(!storage_folder){
            printf("El nombre del folder especificado %s no es válido", storage_folder);
            exit(EXIT_FAILURE);
        }

        // Creamos el folder espacificado previamente
        if(!mkdir_folder(storage_folder)){
            printf("No se ha creado el folder %s para guardar los archivos recibidos\n", storage_folder);
            exit(EXIT_FAILURE);
        }

        else printf("Se ha creado el folder %s para guardar los archivos recibidos\n", storage_folder);

        // Enlistamos los archivos separados por , o ;
        List * requests = list_requests(files);

        // Conectando al servidor
        int client_fd = connect_client(host, port);

        // Por cada archivo se envia una solicitud al servidor
        Node * file_node = requests->start;
        while(file_node != 0){

            // Buffer para colocar la informacion que se lea
            char buffer[BUFFER_SIZE] = {0};
            memset(buffer, 0, BUFFER_SIZE);

            // Obtenemos el lugar donde escribiremos el archivo temporal y el final
            char * absolute_path = get_absolute_path(storage_folder, file_node->content);
            char * tmp_path = get_tmp_path(absolute_path);

            // Se envia la solicitud GET filename HTTP/1.1
            char * http_request = GET(file_node->content);
            write(client_fd, http_request, strlen(http_request));

            // Creamos un archivo temporal para leer del socket
            // Cuando este toda la información se procede a crear el archivo final
            // FILE * tmp_file = fopen(absolute_path, "w");
            char * tmp_filename = get_tmp_path(absolute_path);
            int tmp_file_fd = mkstemp(tmp_filename);
            FILE * tmp_file = fdopen(tmp_file_fd, "w");
            ssize_t written_bytes = read(client_fd, buffer, BUFFER_SIZE);
            while(written_bytes) {
                write(tmp_file_fd, buffer, (size_t) written_bytes);
                written_bytes = read(client_fd, buffer, BUFFER_SIZE);
            }

            // Esta función hace que el archivo temporal sea eliminado al finalizar la ejecución
            unlink(tmp_filename);

            // Colocar el puntero brincandonos el encabezado http
            lseek(tmp_file_fd, 0, SEEK_SET);
            memset(buffer, 0, BUFFER_SIZE);

            // Copiamos lo del archivo temporal al archivo final pero sin el encabezado http
            FILE * final_file = fopen(absolute_path, "w");
            int final_file_fd = fileno(final_file);

            // Obtenenemos el encabezado con la primera recepción
            fread(buffer, 1, BUFFER_SIZE, tmp_file);

            // Dependiendo el tamaño del buffer es posible que vaya parte
            // del contenido del archivo en el mismo buffer.
            char * status = malloc(MAX_FILENAME);
            strcpy(status, strtok(buffer,"\r\n"));
            printf("Estado: %s\n", status);

            // Obtenemos el tipo del archivo, sin embargo, no nos interesa
            // ya que tenemos la extensión del archivo en su nombre
            strtok(0, "\r\n");

            // Obtenemos el tamaño total del archivo que vamos a recibir
            // para saber cuanto mas tenemos que esperar
            char * content_lenght = malloc(MAX_FILENAME);
            strcpy(content_lenght, strtok(0, "\r\n"));
            strtok(content_lenght, ":");
            char * s = strtok(0, " :");
            strcpy(content_lenght, s);
            size_t file_size = (size_t) strtoll(content_lenght, 0, 10);
            printf("Tamaño: %zu\n", file_size);

            fseek(tmp_file, 0L, SEEK_END);
            size_t start_index = (size_t) ftell(tmp_file) - file_size;
            memset(buffer, 0, BUFFER_SIZE);

            fseek(tmp_file, start_index, SEEK_SET);
            written_bytes = fread(buffer, 1, BUFFER_SIZE, tmp_file);
            while(written_bytes) {
                write(final_file_fd, buffer, (size_t) written_bytes);
                written_bytes = fread(buffer, 1, BUFFER_SIZE, tmp_file);
            }

            fclose(final_file);

            if(requests->size == 1){
                char * command = malloc(FILENAME_MAX + 10);
                sprintf(command, "xdg-open %s", absolute_path);
                system(command);
            }

            file_node = file_node->next;
        }

        close(client_fd);

    }

}

addrinfo * get_host_info(char * host, char * port){

    addrinfo hints, * info;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(host, port, &hints, &info) != 0){
        perror("Error: get_host_info(2) \n");
        exit(EXIT_FAILURE);
    }

    return info;
}

int get_client_fd(addrinfo * host_info){

    int family = host_info->ai_family;
    int socktype = host_info->ai_socktype;
    int protocol = host_info->ai_protocol;
    int client_fd = socket(family, socktype, protocol);

    if(client_fd < 0){
        perror("Error: get_client_fd(1) \n");
        exit(EXIT_FAILURE);
    }

    return client_fd;
}
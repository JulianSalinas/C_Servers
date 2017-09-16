#include <sys/stat.h>
#include <pwd.h>
#include "../Headers/client.h"

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
        char * save_folder = argv[2];
        char * files = argv[3];

        if(!is_valid_folder(save_folder)){
            printf("El nombre del folder especificado %s no es válido", save_folder);
            exit(EXIT_SUCCESS);
        }

        // Si la no especifica home, entonces es necesario colocarlo
        if(!strstr(save_folder, "home/")){

            // Obtenemos el directorio home para evitar escribir en las carpetas del sistema
            char * homedir = getenv("HOME");
            if(homedir == 0 ){
                homedir = getpwuid(getuid())->pw_dir;
            }

            // Se concatena home con la folder que especificó el usuario
            char * tmp = save_folder;
            save_folder = malloc(MAX_FILENAME);
            strcpy(save_folder, homedir);
            strcat(save_folder, "/");
            strcat(save_folder, tmp);
            strcat(save_folder, "/");
        }

        printf("Folder para guardar %s\n", save_folder);

        // Si la carpeta para guardar no existe entonces la crea
        struct stat st = {0};
        if(stat(save_folder, &st) < 0){
            if(mkdir(save_folder, 0700)){
                printf("Se ha creado el folder %s para guardar\n", save_folder);
            }
        }

        // Enlistamos los archivos separados por , o ;
        List * requests = new_list();
        char * request = strtok(files, ",");
        while(request){
            add(requests, request);
            request = strtok(NULL,",");
        }
        printf("Archivos solicitados: ");
        print_list(requests, printf);
        printf("\n");

        // Conectando al servidor
        addrinfo * host_info = get_host_info(host, port);
        int client_fd = get_client_fd(host_info);
        connect_client(host_info, client_fd);
        freeaddrinfo(host_info);
        printf("Conectado con %s:%s\n", host, port);

        char buffer[BUFFER_SIZE] = {0};
        char content[BUFFER_SIZE] = {0};

        // Por cada archivo se envia una solicitud al servidor
        Node * file_node = requests->start;
        while(file_node != 0){

            printf(file_node->content);

            // Concatenamos el homedir al filename
            char * filename = malloc(BUFFER_SIZE);
            strcpy(filename, (char *) file_node->content);
            strcat(save_folder, filename);
            strcpy(filename, save_folder);
            printf("Filename %s\n", filename);

            // Se envia la solicitud GET filename HTTP/1.1
            char * http_request = GET(file_node->content);
            write(client_fd, http_request, strlen(http_request));

            // Obtenenemos el encabezado con la primera recepción
            read(client_fd, buffer, BUFFER_SIZE);
            memcpy(content, buffer, BUFFER_SIZE);

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

            FILE * file = fopen(filename, "w");
            if(file){

                size_t i = 0;
                while(content[i] != '\r' && content[i+1] != '\n' && content[i+2] != '\r' && content[i+3] != '\n'){
                    i++;
                }

                fwrite(&content[i+4], 1, file_size, file);



//                size_t read = 0;
//                for (int i = 0; i < BUFFER_SIZE; ++i) {
//                    fwrite(buffer, 1, BUFFER_SIZE, file);
//                }

//                ssize_t ret = 0;
//
//                // Sino, leemos del servidor hasta que no recibamos nada
//                while(read(client_fd, buffer, BUFFER_SIZE)){
//                    //ret = read(client_fd, buffer, BUFFER_SIZE);
//                    //file_size -= ret;
//                    fwrite(buffer, 1, BUFFER_SIZE, file);
//                    memset(buffer, 0, BUFFER_SIZE);
//                }
                fclose(file);
            }
            else {
                printf("Error al guardar el archivo %s", filename);
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

void connect_client(addrinfo * host_info, int client_fd){

    sockaddr * addr = host_info->ai_addr;
    socklen_t socklen = host_info->ai_addrlen;

    if(connect(client_fd, addr, socklen) < 0) {
        perror("Error: connect_client(2) \n");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

}
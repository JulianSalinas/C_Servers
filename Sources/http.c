#include "../Headers/http.h"


char * create_status_code(int status){

    char * response = 0;

    if(status == HTTP_OK)
        response = "HTTP/1.1 200 OK\r\n";
    else if(status == HTTP_NOT_CONTENT)
        response = "HTTP/1.1 204 Not Content\r\n";
    else if(status == HTTP_BAD_REQUEST)
        response = "HTTP/1.1 400 Bad Request\r\n";
    else if(status == HTTP_NOT_FOUND)
        response = "HTTP/1.0 404 Not Found\r\n";
    else if(status == HTTP_UNSUPPORTED_MEDIA_TYPE)
        response = "HTTP/1.1 415 Unsupported Media Type\r\n";
    else if(status == HTTP_SERVER_ERROR)
        response = "HTTP/1.1 500 Internal Server Error\r\n";

    return response;

}

char * create_content_lenght(FILE * file){

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char * content_length = malloc(FILENAME_MAX);
    sprintf(content_length, "Content-Length: %ld\r\n", file_size);

    return content_length;

}

char * create_content_type(char * filename){

    char * extension = 0;

    if(strstr(filename, ".txt"))
        extension = "text/plain";
    else if (strstr(filename, ".html"))
        extension = "text/html";
    else if (strstr(filename, ".png"))
        extension = "image/png";
    else if (strstr(filename, ".jpg"))
        extension = "image/jpg";
    else if (strstr(filename, ".ico"))
        extension = "image/x-icon";
    else return 0;

    char * content_type = malloc(FILENAME_MAX);
    sprintf(content_type, "Content-Type: %s\r\n", extension);

    return content_type;

}

char * create_get_method(char *filename) {

    char * request = malloc(FILENAME_MAX);
    sprintf(request, "GET %s HTTP/1.1\r\n\r\n", filename);
    return request;

}

char * extract_status_code(char * buffer){

    char * cpy_buffer = malloc(BUFFER_SIZE);
    memcpy(cpy_buffer, buffer, BUFFER_SIZE);

    char * status = malloc(MAX_FILENAME);
    strcpy(status, strtok(cpy_buffer,"\r\n"));
    return status;

}

size_t extract_content_lenght(char * buffer){

    char * cpy_buffer = malloc(BUFFER_SIZE);
    memcpy(cpy_buffer, buffer, BUFFER_SIZE);
    strtok(cpy_buffer,"\r\n");
    strtok(0,"\r\n");

    char * content_lenght = malloc(MAX_FILENAME);
    strcpy(content_lenght, strtok(0, "\r\n"));

    strtok(content_lenght, ":");
    strcpy(content_lenght, strtok(0, " :"));

    return (size_t) strtoll(content_lenght, 0, 10);

}
#include "../../Headers/Util/util.h"

int is_valid_filename(char * filename){

    int is_valid = 1;
    char bad_chars[] = "!@%^*~|";

    for (int i = 0; i < strlen(bad_chars); ++i) {
        if(strchr(filename, bad_chars[i]) != 0){
            is_valid = 0;
            break;
        }
    }

    return is_valid;
}

int is_valid_folder(char * foldername){

    int is_valid = 1;
    char bad_chars[] = "!@%^*~|\\#.";

    for (int i = 0; i < strlen(bad_chars); ++i) {
        if(strchr(foldername, bad_chars[i]) != 0){
            is_valid = 0;
            break;
        }
    }

    return is_valid;
}
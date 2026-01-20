#include "get_input.h"


static char* buff = NULL;
static size_t buff_capacity = 8;

char* get_input() {
    
    if(buff == NULL) {
        buff = malloc(buff_capacity);
    }
    
    memset(buff, 0, buff_capacity);
    write(STDOUT_FILENO, "kell$ ", 6);

    int total_bytes = 0;
    
    while(1) {

        int bytes_read = read(STDIN_FILENO, buff + total_bytes, buff_capacity - 1 - total_bytes);

        if (bytes_read < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("kell: read");
            exit(1);
        }

        if (bytes_read == 0) {
            if (total_bytes == 0) {
                exit(0);
            }
            break;
        } 

        total_bytes += bytes_read;
        buff[total_bytes] = '\0';

        if (buff[total_bytes - 1] == '\n') {
            buff[total_bytes - 1] = '\0';
            if (total_bytes == 1) { // user only pressed enter get another input
                total_bytes = 0;
                continue;
            }
            break;
        }

        

        if (buff_capacity == total_bytes + 1) {
            char* tmp = resize(buff, buff_capacity * 2);
            if (tmp != NULL) {
                buff = tmp;
                buff_capacity *= 2;                                                         
            } else {
                perror("kell: resize");
                exit(1);
            }
        }

    }
    return buff;
}

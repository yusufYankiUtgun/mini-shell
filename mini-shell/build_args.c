#include "build_args.h"

int build_args(char*** addr_args, char** addr_buff, size_t* addr_buff_capacity, size_t* addr_args_number, int* addr_eof_flag) {

    char** args = *addr_args;
    char* buff = *addr_buff;
    size_t buff_capacity = *addr_buff_capacity;
    size_t args_number = *addr_args_number;

    

    while (1) {

        memset(args, 0, args_number * sizeof(char*));
        memset(buff, 0, buff_capacity);
        
        (*addr_eof_flag) = 1;
        
        printf("kell$ ");
        fflush(stdout);

        int bytes_read = read(0, buff, buff_capacity - 1);
        
        if (bytes_read < 0) {  // actual error (not eof)
            perror("kell: read");
            continue; // get another input
        }

        if (bytes_read == 0) { // eof 
            printf("\n");
            return 1; // exit in kell.c 
        }
        
        buff[bytes_read] = '\0';
        
        while(buff[strlen(buff) - 1] != '\n') { // continue reading to buffer if input is not finished by new
            int current_length = strlen(buff);

            if (strlen(buff) == buff_capacity - 1) {
                char* tmp = resize(buff, buff_capacity * 2);
                
                if (tmp != NULL) {
                    buff = tmp;
                    buff_capacity *= 2;                                                         
                } else {
                    perror("kell: resize");
                    break;
                }
            }
            
            bytes_read = read(0 , buff + current_length, buff_capacity - current_length - 1);
            
            if (bytes_read == 0) {
                (*addr_eof_flag) = 0;
                break;
            }
            buff[current_length + bytes_read] = '\0';
        }
        
        int a = 0;
        
        if ((*addr_eof_flag)) {
            buff[strlen(buff) - 1] = '\0';
        }
        
        char* token = strtok(buff, " \t\n\r");

        int args_capacity_flag = 0;

        while (token != NULL) {
            if (a == args_number) {
                char** tmp = resize(args, 2 * args_number * sizeof(char*));

                if (tmp != NULL) {
                    args = tmp;
                    args_number *= 2;
                } else {
                    perror("kell: resize");
                    args_capacity_flag = 1;
                    break;
                }
            }
            
            args[a++] = token;
            token = strtok(NULL, " \t\n\r");
        }

        if (args_capacity_flag) {
            continue; // get another input 
        }

        args[a] = NULL;

        if (a == 0) {
            continue; // no arguments passed get another input
        }

        break;

    }

    // prevent dangling caused by realloc
    *addr_buff = buff; 
    *addr_args = args;

    return 0;

}


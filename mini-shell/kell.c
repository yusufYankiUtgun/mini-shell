#include "kell.h"

void* resize(void* ptr, size_t new_size) {
    void* temp_ptr = realloc(ptr, new_size);

    if (temp_ptr == NULL) {
        perror("kell: realloc");
        return NULL;
    }
    return temp_ptr;
}

int main(int argc, char** argv) {
    size_t buff_capacity = 64 * sizeof(char);
    char* buff = malloc(buff_capacity);
    size_t args_number = 8;
    char** args_to_send_exec = malloc(args_number * sizeof(char*));

    while (1) {
        memset(args_to_send_exec, 0, args_number * sizeof(char*));
        memset(buff, 0, buff_capacity);
        
        int eof_flag = 1;
        
        printf("kell$ ");
        fflush(stdout);

        int bytes_read = read(0, buff, buff_capacity - 1);
        
        if (bytes_read == 0) {
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        
        buff[bytes_read] = '\0';
        
        while(buff[strlen(buff) - 1] != '\n') {
            int current_length = strlen(buff);

            if (strlen(buff) == buff_capacity - 1) {
                char* tmp = resize(buff, buff_capacity * 2);
                
                if (tmp != NULL) {
                    buff = tmp;
                    buff_capacity *= 2;                                                         
                } else {
                    printf("not enough memory");
                    break;
                }
            }
            
            bytes_read = read(0 , buff + current_length, buff_capacity - current_length - 1);
            
            if (bytes_read == 0) {
                eof_flag = 0;
                break;
            }
            buff[current_length + bytes_read] = '\0';
        }
        
        int a = 0;
        
        if (eof_flag) {
            buff[strlen(buff) - 1] = '\0';
        }
        
        char* token = strtok(buff, " ");

        int args_capacity_flag = 0;

        while (token != NULL) {
            if (a == args_number) {
                char** tmp = resize(args_to_send_exec, 2 * args_number * sizeof(char*));

                if (tmp != NULL) {
                    args_to_send_exec = tmp;
                    args_number *= 2;
                } else {
                    printf("not enough memory");
                    args_capacity_flag = 1;
                    break;
                }
            }
            
            args_to_send_exec[a++] = token;
            token = strtok(NULL, " ");
            
        }

        if (args_capacity_flag) {
            continue;
        }

        args_to_send_exec[a] = NULL;

        if (a == 0) {
            continue;
        }

        pid_t cpid = fork();
        
        if (cpid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        int status;

        if (cpid == 0) {
            if (!eof_flag) {
                printf("\n");
            } 
            if (execvp(args_to_send_exec[0], args_to_send_exec) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } 
        wait(&status);
    }
    return EXIT_SUCCESS;
}

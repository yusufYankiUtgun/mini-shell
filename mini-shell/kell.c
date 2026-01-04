#include "kell.h"

void* resize(void* ptr, size_t new_size) {
    void* temp_ptr = realloc(ptr, new_size);

    if (temp_ptr == NULL) {
        perror("kell: realloc");
    }
    return temp_ptr;
}

int main(int argc, char** argv) {
    size_t buff_capacity = INITIAL_BUFF_SIZE * sizeof(char);
    char* buff = malloc(buff_capacity);
    size_t args_number = INITIAL_ARGS_SIZE;
    char** args = malloc(args_number * sizeof(char*));

    while (1) {
        memset(args, 0, args_number * sizeof(char*));
        memset(buff, 0, buff_capacity);
        
        int eof_flag = 1;
        
        printf("kell$ ");
        fflush(stdout);

        int bytes_read = read(0, buff, buff_capacity - 1);
        
        if (bytes_read < 0) {  // actual error (not eof)
            perror("kell: read");
            continue;
        }

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
                    perror("kell: resize");
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
            continue;
        }

        args[a] = NULL;

        if (a == 0) {
            continue;
        }


        int status = handle_builtins(args); // 0 stop | 1 builtins runned, continue | -1 fork and execute
        
        if (status == BUILTIN_EXIT) { // free the memory then exit
            
            long val = 0; //default val

            if (args[1] != NULL) {
                char* endptr;
                val = strtol(args[1], &endptr, 10);

                if (!(args[1][0] != '\0' && *endptr == '\0')) { // checks if the argument is number
                    val = 2;  // exits with code 2 on no numeric argument
                } else if (args[2] != NULL) {
                    fprintf(stderr, "kell: exit: too many arguments\n");
                    continue;
                }
            }
            
            free(buff);
            free(args);
            exit((int)val);
            
        } else if (status == BUILTIN_NOT_FOUND) { // fork and exec then wait for new command

            pid_t cpid = fork();
        
            if (cpid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            
            int exit_status;

            if (cpid == 0) {
                if (!eof_flag) {
                    printf("\n");   // to move to a new line on execution with eof
                } 
                if (execvp(args[0], args) == -1) {
                    fprintf(stderr, "kell: %s: command not found\n", args[0]);
                    exit(127); // default command not found code 127
                }
            } 
            
            wait(&exit_status);

        }   // status is 1 and builtin commands are done now program can wait for new command

        
    }
    free(buff);
    free(args);
    return EXIT_SUCCESS;
}

#include "kell.h"

int main(int argc, char** argv) {
    size_t buff_capacity = INITIAL_BUFF_SIZE * sizeof(char);
    char* buff = malloc(buff_capacity);
    if (buff == NULL) {
        perror("kell: malloc");
    }
    size_t args_number = INITIAL_ARGS_NUMBER;
    char** args = malloc(args_number * sizeof(char*));
    if (args == NULL) {
        perror("kell: malloc");
    }

    int eof_flag;

    while (1) {
        
        int build_args_status = build_args(&args, &buff, &buff_capacity, &args_number, &eof_flag);

        if (build_args_status) {
            exit(EXIT_SUCCESS);
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

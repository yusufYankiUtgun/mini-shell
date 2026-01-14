#include "handle_builtins.h"

int handle_builtins(char** args) {

    if (strcmp(args[0], "exit") == 0) {
        return BUILTIN_EXIT;
     }

    if (strcmp(args[0], "cd") == 0) {
        if (args[2] != NULL) {
            fprintf(stderr, "kell: cd: too many arguments\n");
        } else {
            int status = chdir(args[1]);
        
            if (status == -1) {
                fprintf(stderr, "kell: cd: %s: %s\n", args[1], strerror(errno));
            }

        }
        return BUILTIN_HANDLED;
    }
    
    if (strcmp(args[0], "pwd") == 0) {

        size_t size = INITIAL_PATH_SIZE;
        size_t buf_capacity = size * sizeof(char);
        char* buf = malloc(buf_capacity);
        
        if (buf == NULL) {
            perror("kell: malloc");
        }

        char* tmp = buf;

        while (getcwd(buf, size) == NULL) {

            if (errno == ERANGE) {
                tmp = resize(buf, buf_capacity * 2);
                if (tmp != NULL) {
                    buf = tmp;
                    buf_capacity *= 2;
                    size *= 2;
                } else {
                    perror("kell: resize");
                    break;
                }

            } else {
                perror("kell: getcwd");
                break;
            }

        }
        write(STDOUT_FILENO, "kell: ", 6);
        write(STDOUT_FILENO, buf, strlen(buf));
        write(STDOUT_FILENO, "\n", 1);
        free(buf);
        return BUILTIN_HANDLED;
    }

    if (strcmp(args[0], "echo") == 0) {
        int new_line_flag = 1;
        int a = 1;
                 
        if (strcmp(args[1], "-n") == 0) {
            new_line_flag == 0;
            a = 2;
        }

        while (args[a] != NULL) {
            write(STDOUT_FILENO, args[a], strlen(args[a])); 
            
            if (args[a + 1] != NULL) {
                write(STDOUT_FILENO, " ", 1);
            }
            a++;
        }

        if (new_line_flag) {
            write(STDOUT_FILENO, "\n", 1);
        }

        return BUILTIN_HANDLED;
    }



    return BUILTIN_NOT_FOUND;
}
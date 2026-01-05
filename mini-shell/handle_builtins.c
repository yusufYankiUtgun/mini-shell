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

        fprintf(stdout, "kell: %s\n", buf);
        free(buf);
        return BUILTIN_HANDLED;
    }


    return BUILTIN_NOT_FOUND;
}
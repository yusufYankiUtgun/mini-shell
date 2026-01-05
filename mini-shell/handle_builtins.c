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



    return BUILTIN_NOT_FOUND;
}
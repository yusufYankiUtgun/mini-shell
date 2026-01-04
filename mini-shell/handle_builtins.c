#include "handle_builtins.h"

int handle_builtins(char** args) {

    if (strcmp(args[0], "exit") == 0) {
        return BUILTIN_EXIT;
    }

    return BUILTIN_NOT_FOUND;
}
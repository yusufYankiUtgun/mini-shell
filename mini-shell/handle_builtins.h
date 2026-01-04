#ifndef HANDLE_BUILTINS_H
#define HANDLE_BUILTINS_H

#include<stdio.h>
#include<string.h>

typedef enum {
    BUILTIN_EXIT = 0,
    BUILTIN_HANDLED = 1,
    BUILTIN_NOT_FOUND = -1
} builtin_status_t;

builtin_status_t handle_builtins(char** args);

#endif
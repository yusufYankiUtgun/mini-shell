#ifndef PARSER_H 
#define PARSER_H 

#include <stdio.h>
#include "lexer.h"
#include "resize.h"
#define INITIAL_STRING_SIZE 8
#define INITIAL_ARGS_SIZE 4

typedef struct command command;

struct command {
    char* command_name;
    char** args;
    char* input_source;
    int input_type;
    char* output_destination;
    int output_type;
    command* next_command;
};


typedef enum {
    STANDART_IO = 0, 
    REDIRECTED_IO = 1, 
    PIPED_IO = 2,
    APPEND_IO = 3
} t_IO_type;


command* build_commands(token* tokens);
command* create_new_command(command* current);


#endif
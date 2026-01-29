#ifndef LEXER_H
#define LEXER_H 

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    TOKEN_WORD = 0,
    TOKEN_PIPE = 1, 
    TOKEN_REDIRECT_IN = 2,
    TOKEN_REDIRECT_OUT = 3,
    TOKEN_REDIRECT_APPEND = 4
} t_token_type; 

typedef struct token token;

struct token {
    char* value;
    t_token_type type;
    token* next;
};

token* tokenize_input(char* input);
void free_tokens(token* head);


#endif 
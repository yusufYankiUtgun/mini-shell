#include "lexer.h"

token* tokenize_input(char* input) {

    token* curr_token = malloc(sizeof(token));
    token* head = curr_token;
    head->value = NULL;
    head->next = NULL;

    int i = 0;
    int is_inside_quotes = 0;
    int word_length = 0;
    int word_start_index = 0;
    int word_started = 0;
    int should_break = 0;


    while (input[i] != '\0') {
        
        char curr_char = input[i];

        if (is_inside_quotes == 1) {
            if (curr_char == '\'' || curr_char == '\"') {
                curr_token->type = TOKEN_WORD;
                curr_token->value = strndup(input + word_start_index, word_length);
                token* tmp = malloc(sizeof(token));
                tmp->next = NULL;
                tmp->value = NULL;
                curr_token->next = tmp;
                curr_token = tmp;

                word_length = 0;
                is_inside_quotes = 0;
            }
            
            word_length++;
        } else {
            
            if (isblank(curr_char) == 1) {
                if (word_started == 1) {
                    curr_token->type = TOKEN_WORD;
                    curr_token->value = strndup(input + word_start_index, word_length);
                    token* tmp = malloc(sizeof(token));
                    tmp->next = NULL;
                    tmp->value = NULL;
                    curr_token->next = tmp;
                    curr_token = tmp;

                    word_length = 0;
                    word_started = 0;
                } else {
                    i++;
                    continue;
                }

            } else if (curr_char == '\'' || curr_char == '\"') {
                
                if (word_started == 1) {
                    return head;
                } else {
                    is_inside_quotes = 1;
                    word_start_index = i + 1;    
                }
                  
            } else if (curr_char == '|') {
                if (word_started == 1) { // written inside or end of the word
                    fprintf(stderr, "kell: syntax error: |\n");
                    free_tokens(head);
                    return NULL;
                } else {
                    if (input[i + 1] != '\0' && isblank(input[i + 1]) == 0) { // written beginning of the word;
                        fprintf(stderr, "kell: syntax error: |\n");
                        free_tokens(head);
                        return NULL;
                    } else {
                        curr_token->type = TOKEN_PIPE;
                        char* val_tmp = malloc(2);
                        strcpy(val_tmp, "|\0");
                        curr_token->value = val_tmp;
                        token* tmp = malloc(sizeof(token));
                        tmp->next = NULL;
                        tmp->value = NULL;
                        curr_token->next = tmp;
                        curr_token = tmp;

                        word_length = 0;
                    }

                }      
            } else if (curr_char == '<') {
                if (word_started != 0) { // written inside or end of the word
                    fprintf(stderr, "kell: syntax error: <\n");
                    free_tokens(head);
                    return NULL;
                } else {
                    if (input[i + 1] != '\0' && isblank(input[i + 1]) == 0) { // written beginning of the word;
                        fprintf(stderr, "kell: syntax error: <\n");
                        free_tokens(head);
                        return NULL;
                    } else {
                        curr_token->type = TOKEN_REDIRECT_IN;
                        char* val_tmp = malloc(2);
                        strcpy(val_tmp, "<\0");
                        curr_token->value = val_tmp;
                        token* tmp = malloc(sizeof(token));
                        tmp->next = NULL;
                        tmp->value = NULL;
                        curr_token->next = tmp;
                        curr_token = tmp;

                        word_length = 0;
                    }

                }
            } else if (curr_char == '>') {
                if (input[i + 1] != '>') { // not >>
                    if (word_started != 0) { // written inside or end of the word
                        fprintf(stderr, "kell: syntax error: >\n");
                        free_tokens(head);
                        return NULL;
                    } else {
                        if (input[i + 1] != '\0' && isblank(input[i + 1]) == 0) { // written beginning of the word;
                            fprintf(stderr, "kell: syntax error: >\n");
                            free_tokens(head);
                            return NULL;
                        } else {
                            curr_token->type = TOKEN_REDIRECT_OUT;
                            char* val_tmp = malloc(2);
                            strcpy(val_tmp, ">\0");
                            curr_token->value = val_tmp;
                            token* tmp = malloc(sizeof(token));
                            tmp->next = NULL;
                            tmp->value = NULL;
                            curr_token->next = tmp;
                            curr_token = tmp;

                            word_length = 0;
                        }
                    }

                } else { // >> 
                    if (word_started != 0) { // written inside or end of the word
                        fprintf(stderr, "kell: syntax error: >>\n");
                        free_tokens(head);
                        return NULL;
                    } else {
                        if (input[i + 2] != '\0' && isblank(input[i + 2]) == 0) { // written beginning of the word;
                            fprintf(stderr, "kell: syntax error: >>\n");
                            free_tokens(head);
                            return NULL;
                        } else {
                            curr_token->type = TOKEN_REDIRECT_APPEND;
                            char* val_tmp = malloc(3);
                            strcpy(val_tmp, ">>\0");
                            curr_token->value = val_tmp;
                            token* tmp = malloc(sizeof(token));
                            tmp->next = NULL;
                            tmp->value = NULL;
                            curr_token->next = tmp;
                            curr_token = tmp;
                            i++;
                            word_length = 0;
                        }
                    }
                }
            } else {
                // start word and set start index
                if (word_started != 1) {
                    word_start_index = i;   
                    word_started = 1;    
                }
                word_length++;
            }
        }
        i++;
    }

    
    // if input ends while still inside quotes return NULL to signal an error
    if (is_inside_quotes == 1) {
        fprintf(stderr, "kell: lexer: unclosed quote\n");
        free_tokens(head);
        return NULL;
    }

    // handles last word
    if (word_started == 1) {
        curr_token->type = TOKEN_WORD;
        curr_token->value = strndup(input + word_start_index, word_length);
        token* tmp = malloc(sizeof(token));
        tmp->next = NULL;
        tmp->value = NULL;
        curr_token->next = tmp;
    }
    return head;
}

void free_tokens(token* walk) {
    token* tmp;
    
    while (walk != NULL) {
        tmp = walk->next;
        
        if (walk->value != NULL) {
            free(walk->value);
        }

        free(walk);
        walk = tmp;
    }
}




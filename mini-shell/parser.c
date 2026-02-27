#include "parser.h"
command* build_commands(token* walk) {
 
    token* token_head = walk;

    int args_size = INITIAL_ARGS_SIZE;
    int curr_args_size = 0;
    int command_started = 0;
    int is_prev_word = 1;

    command* head = malloc(sizeof(command));
    head->args = malloc(INITIAL_ARGS_SIZE * sizeof(char*));
    command* curr_command = head;
    
    curr_command->input_type = STANDART_IO;
    curr_command->output_type = STANDART_IO;
    curr_command->next_command = NULL;
    head->input_source = NULL;
    head->output_destination = NULL;
    head->command_name = NULL;


    if (walk->type == TOKEN_PIPE) {
        fprintf(stderr, "kell: syntax error: input starts with |\n");
        //WIP decide what to do after error
    }

    while (walk->type != TOKEN_EMPTY) {

        if (command_started) {
            if (walk->type == TOKEN_WORD) {
                
                if (curr_args_size == 0) {
                    curr_command->command_name = strdup(walk->value);
                } 

                if (curr_args_size + 1 >= args_size) {
                    char** tmp;

                    tmp = resize(curr_command->args, args_size * 2 * sizeof(char*));

                    if (tmp == NULL) {
                        fprintf(stderr, "kell: resize");
                        exit(0);
                    } else {
                        curr_command->args = tmp;
                        args_size *= 2;
                    }
                }
                is_prev_word = 1;
                curr_command->args[curr_args_size] = strdup(walk->value);
                curr_args_size++;

            } else if (walk->type == TOKEN_PIPE) {
                
                if (!is_prev_word) {
                    fprintf(stderr, "kell: syntax error: need word after special character 1 ");
                    //WIP
                }
                is_prev_word = 0;

                if (walk->next->type == TOKEN_EMPTY) {
                    fprintf(stderr, "kell: syntax error: input ends with special character ");
                    //WIP
                }


                curr_command->output_type = PIPED_IO;
                curr_command->args[curr_args_size] = NULL;

                curr_command = create_new_command(curr_command);

                curr_command->input_type = PIPED_IO;
                
                command_started = 0;
                curr_args_size = 0;
                args_size = INITIAL_ARGS_SIZE;

            } else if (walk->type == TOKEN_REDIRECT_IN) {

                if (!is_prev_word) {
                    fprintf(stderr, "kell: syntax error: need word after special character 2 ");
                    //WIP
                }
                is_prev_word = 0;

                if (walk->next->type == TOKEN_EMPTY) {
                    fprintf(stderr, "kell: syntax error: input ends with special character");
                    //WIP
                }

                curr_command->input_type = REDIRECTED_IO;
                
                if (walk->next->type == TOKEN_WORD) {
                    walk = walk->next;
                    is_prev_word = 1;
                } else {
                    fprintf(stderr, "kell: syntax error: need a file name after redirect");
                }
                
                curr_command->input_source = strdup(walk->value);

            } else if (walk->type == TOKEN_REDIRECT_OUT) {
                

                if (!is_prev_word) {
                    fprintf(stderr, "kell: syntax error: need word after special character 3 ");
                    //WIP
                }
                is_prev_word = 0;

                if (walk->next->type == TOKEN_EMPTY) {
                    fprintf(stderr, "kell: syntax error: input ends with special character");
                    //WIP
                }

                if (walk->next->type == TOKEN_WORD) {
                    walk = walk->next;
                    is_prev_word = 1;
                } else {
                    fprintf(stderr, "kell: syntax error: need a file name after redirect");
                }

                curr_command->output_type = REDIRECTED_IO;
                curr_command->output_destination = strdup(walk->value);

            } else if (walk->type == TOKEN_REDIRECT_APPEND) {
                
                if (!is_prev_word) {
                    fprintf(stderr, "kell: syntax error: need word after special character 4 ");
                    //WIP
                }
                is_prev_word = 0;

                if (walk->next->type == TOKEN_EMPTY) {
                    fprintf(stderr, "kell: syntax error: input ends with special character");
                    //WIP
                }


                if (walk->next->type == TOKEN_WORD) {
                    walk = walk->next;
                    is_prev_word = 1;
                } else {
                    fprintf(stderr, "kell: syntax error: need a file name after redirect");
                }

                curr_command->output_type = APPEND_IO;
                curr_command->output_destination = strdup(walk->value);


            }

        } else {

            if (walk->type == TOKEN_WORD) {
                curr_command->command_name = strdup(walk->value);
                curr_args_size += 1;
                
                *(curr_command->args) = strdup(walk->value);
                command_started = 1;
                is_prev_word = 1;

            } else if (walk->type == TOKEN_REDIRECT_IN) {

                is_prev_word = 0;

                if (walk->next->type == TOKEN_EMPTY) {
                    fprintf(stderr, "kell: syntax error: input ends with special character");
                    //WIP
                }

                if (walk->next->type == TOKEN_WORD) {
                    walk = walk->next;
                    is_prev_word = 1;
                } else {
                    fprintf(stderr, "kell: syntax error: need a file name after redirect");
                }

                curr_command->input_type = REDIRECTED_IO;
                curr_command->input_source = strdup(walk->value);
                command_started = 1;

            } else if (walk->type == TOKEN_REDIRECT_OUT) {

                is_prev_word = 0;
                
                if (walk->next->type == TOKEN_EMPTY) {
                    fprintf(stderr, "kell: syntax error: input ends with special character");
                    //WIP
                }

                if (walk->next->type == TOKEN_WORD) {
                    walk = walk->next;
                    is_prev_word = 1;
                } else {
                    fprintf(stderr, "kell: syntax error: need a file name after redirect");
                }

                curr_command->output_type = REDIRECTED_IO;
                curr_command->output_destination = strdup(walk->value);
                command_started = 1;


            } else if (walk->type == TOKEN_REDIRECT_APPEND){

                is_prev_word = 0;

                if (walk->next->type == TOKEN_EMPTY) {
                    fprintf(stderr, "kell: syntax error: input ends with special character");
                    //WIP
                }

                if (walk->next->type == TOKEN_WORD) {
                    walk = walk->next;
                    is_prev_word = 1;
                } else {
                    fprintf(stderr, "kell: syntax error: need a file name after redirect");
                }

                curr_command->output_type = APPEND_IO;
                curr_command->output_destination = strdup(walk->value);
                command_started = 1;

            } else if (walk->type == TOKEN_PIPE) {

                fprintf(stderr, "kell: syntax error: need a command before |\n");
                //WIP decide what to do after error
                
                if (walk->next->type == TOKEN_EMPTY) {
                    fprintf(stderr, "kell: syntax error: input ends with special character");
                    //WIP
                }

            }
        } 
        walk = walk->next;
    }
    curr_command->args[curr_args_size] = NULL;
    free_tokens(token_head);
    return head;
}

command* create_new_command(command* current) {

    command* new_command = malloc(sizeof(command));
    new_command->args = malloc(INITIAL_ARGS_SIZE * sizeof(char*));
    new_command->input_type = STANDART_IO;
    new_command->output_type = STANDART_IO;
    new_command->next_command = NULL;
    current->next_command = new_command;
    new_command->input_source = NULL;
    new_command->output_destination = NULL;
    new_command->command_name = NULL;
    
    return new_command;
}
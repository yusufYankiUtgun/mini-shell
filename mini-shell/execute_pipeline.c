#include "execute_pipeline.h"

void execute_pipeline(command* head) {

    command* walk = head;
    

    int read_end;

    if (walk->input_type == REDIRECTED_IO) {
        int fd = open(walk->input_source, O_RDONLY);
        
        if (fd < 0) {
            fprintf(stderr, "kell: %s: no such file\n", walk->input_source);
            return;
        }
        
        read_end = fd;
    } else {
        read_end = STDIN_FILENO;
    }

    while (walk->output_type == PIPED_IO) {

        int fd[2];
        pipe(fd);
        
        pid_t cpid = fork();

        if (cpid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (cpid == 0) {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            
            if (read_end != STDIN_FILENO) {
                dup2(read_end, STDIN_FILENO);
                close(read_end);
            }
            
            if (execvp(walk->command_name, walk->args) == -1) {
                fprintf(stderr, "kell: %s: command not found\n", walk->command_name);
                exit(127); // default command not found code 127
            }
        } else {
            if (read_end != STDIN_FILENO) {
                close(read_end);    
            }
            read_end = fd[0];
            close(fd[1]);
            walk = walk->next_command;
        }
    }

    execute_redirected_command(walk, read_end);
    
    while (wait(NULL) > 0);

    free_commands(head);
}

void free_commands(command* head) {

    while (head != NULL) {

        command* next = head->next_command;

        free(head->command_name);
        free(head->input_source);
        free(head->output_destination);

        char* args_head = head->args[0];

        for (int i = 0; head->args[i] != NULL; i++) {
            free(head->args[i]);
        }
        free(head->args);
        free(head);

        head = next;
    }
}
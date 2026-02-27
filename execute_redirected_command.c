#include "execute_redirected_command.h"


void execute_redirected_command(command* com, int read_end)   {

    pid_t cpid = fork();
    int exit_status;

    if (cpid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {

        if (read_end != STDIN_FILENO) {
            dup2(read_end, STDIN_FILENO);
            close(read_end);
        }   

        
        if (com->input_type == REDIRECTED_IO) {
            
            int fd = open(com->input_source, O_RDONLY);
            
            if (fd < 0) {
                fprintf(stderr, "kell: %s: no such file\n", com->input_source);
                exit(1);
            }
            
            dup2(fd, STDIN_FILENO);
            close(fd);

        }

        if (com->output_type == REDIRECTED_IO) {
            
            int fd = open(com->output_destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
            if (fd < 0) {
                perror("kell: open");
                exit(1);
            }
            
            dup2(fd , STDOUT_FILENO);
            close(fd);

        } else if (com->output_type == APPEND_IO) {

            int fd = open(com->output_destination, O_WRONLY | O_CREAT | O_APPEND, 0644);
            
            if (fd < 0) {
                perror("kell: open");
                exit(1);
            }
            
            dup2(fd , STDOUT_FILENO);
            close(fd);
        }
        
        if (execvp(com->command_name, com->args) == -1) {
            fprintf(stderr, "kell: %s: command not found\n", com->command_name);
            exit(127); // default command not found code 127
        }

    }

    if (read_end != STDIN_FILENO) {
        close(read_end);
    }   


    wait(&exit_status);



}

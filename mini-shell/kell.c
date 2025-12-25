#include "kell.h"

int main(int argc, char** argv) {

    char buff[1024];

    while (1) {
        
        printf("kell$ ");
        fgets(buff, BUFF_MAX, stdin);
        char* argsToSendExec[MAX_ARGC];
        
        int a = 0;
        buff[strlen(buff) - 1] = '\0';
        char* token = strtok(buff, " ");

        

        while (token != NULL) {

            argsToSendExec[a++] = token;
            token = strtok(NULL, " ");

        }

        argsToSendExec[a] = NULL;

        pid_t cpid = fork();
        



        if (cpid < 0) {
            perror("fork");
            return EXIT_FAILURE;
        }
        
        int status;

        if (cpid == 0) {

            if (execvp(argsToSendExec[0], argsToSendExec) == -1) {
                perror("execvp");
                return EXIT_FAILURE;
            }

        } 

        wait(&status);
    }
        



        
    




    return EXIT_SUCCESS;
}

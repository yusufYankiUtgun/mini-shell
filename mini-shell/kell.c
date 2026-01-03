#include "kell.h"




void* resize(void* ptr, size_t new_size) {

    void* temp_ptr = realloc(ptr, new_size);

    if (temp_ptr == NULL) {
        perror("kell: realloc");
        return NULL;
    }
    
    return temp_ptr;

}



int main(int argc, char** argv) {

    size_t buff_capacity = 2 * sizeof(char);
    char* buff = malloc(buff_capacity);
    

    while (1) {
        
        int flag = 1;

        printf("kell$ ");
        fflush(stdout);


        int bytes_read = read(0, buff, buff_capacity - 1);
        
        if (bytes_read == 0) {
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        
        buff[bytes_read] = '\0';
        
        while(buff[strlen(buff) - 1] != '\n') {
            
            int current_length = strlen(buff);

            if (strlen(buff) == buff_capacity - 1) {
                
                char* tmp = resize(buff, buff_capacity * 2);
                
                
                if (tmp != NULL) {
                    buff = tmp;
                    buff_capacity *= 2;                                                         
                } else {
                    printf("not enough memory");
                    continue;
                }

            }
            

            bytes_read = read(0 , buff + current_length, buff_capacity - current_length - 1);
            
            if (bytes_read == 0) {
                flag = 0;
                break;
            }
        
            buff[current_length + bytes_read] = '\0';
            

        }
        

        char* argsToSendExec[MAX_ARGC];
        
        int a = 0;
        
        if (flag) {
            buff[strlen(buff) - 1] = '\0';
        }
        
        char* token = strtok(buff, " ");

        
        while (token != NULL) {

            argsToSendExec[a++] = token;
            token = strtok(NULL, " ");

        }

        argsToSendExec[a] = NULL;

        if (a == 0) {
            continue;
        }

        pid_t cpid = fork();
        

        if (cpid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        int status;

        if (cpid == 0) {
            if (!flag) {
                printf("\n");
            } 
            if (execvp(argsToSendExec[0], argsToSendExec) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }

        } 

        wait(&status);
    }
    return EXIT_SUCCESS;
}

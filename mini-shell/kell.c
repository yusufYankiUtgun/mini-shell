#include "kell.h"

int main(int argc, char** argv) {
    
    char* buff = get_input();

    printf("%s\n" ,buff);
    fflush(stdout);

    token* tok = tokenize_input(buff);
    token* walk = tok;
    


    while (walk != NULL) {

        printf("%s %d\n", walk->value, walk->type);

        walk = walk->next;
    }
    
}

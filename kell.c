#include "kell.h"

int main(int argc, char** argv) {
    
    
    while (1) {

        char* buff = get_input();

        token* token_head = tokenize_input(buff);
        command* command_head = build_commands(token_head);

        execute_pipeline(command_head);

    }
}

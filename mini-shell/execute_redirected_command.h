
#ifndef EXECUTE_REDIRECTED_COMMAND_H
#define EXECUTE_REDIRECTED_COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include "resize.h"
#include <sys/wait.h>

#include "lexer.h"
#include "parser.h"
void execute_redirected_command(command* com, int read_end);


#endif
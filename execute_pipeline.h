#ifndef EXECUTE_PIPELINE_H
#define EXECUTE_PIPELINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include "resize.h"
#include "execute_redirected_command.h"
#include "lexer.h"
#include "parser.h"
#include <sys/wait.h>

void execute_pipeline(command* head);
void free_commands(command* head);

#endif
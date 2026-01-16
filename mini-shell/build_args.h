#ifndef BUILD_ARGS_H
#define BUILD_ARGS_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "resize.h"

int build_args(char*** addr_args, char** addr_buff, 
                size_t* addr_buff_capacity, size_t* addr_args_number, int* addr_eof_flag);


#endif
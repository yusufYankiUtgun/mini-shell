#include "resize.h"


void* resize(void* ptr, size_t new_size) {
    void* temp_ptr = realloc(ptr, new_size);

    if (temp_ptr == NULL) {
        perror("kell: realloc");
    }
    return temp_ptr;


}
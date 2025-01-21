
#include <iostream>
#include <unistd.h>

#define MAX_SIZE 100000000

void* smalloc(size_t size){
    if(size == 0 || size > MAX_SIZE){
        return nullptr;
    }
    void* ret_val = sbrk(size);
    if(ret_val == (void*)-1){
        return NULL;
    }
    return ret_val;
}
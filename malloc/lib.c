#include <dlfcn.h>
#include "stdlib.h"
#include "string.h"

void* malloc(size_t size) {
    void* ptr = calloc(1, size);
    memset(ptr, 0xFF, size);
    return ptr;
}
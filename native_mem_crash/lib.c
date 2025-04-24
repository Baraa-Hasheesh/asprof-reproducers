#include "lib.h"

EXPORT void* my_malloc(size_t size) {
    return malloc(size);
}

EXPORT void my_free(void* ptr) {
    free(ptr);
}
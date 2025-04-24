#ifndef MYLIB_H
#define MYLIB_H

#define EXPORT __attribute__((visibility("default")))

#include "stdlib.h"


EXPORT void* my_malloc(size_t size);

EXPORT void my_free(void* ptr);

#endif
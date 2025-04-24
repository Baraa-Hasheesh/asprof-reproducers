#include "lib_dlopen.h"
#include <dlfcn.h>
#include <stdio.h>

EXPORT void* my_dlopen(char* path, int options) {
    printf("libMy_dlopen: %s\n", path);
    return dlopen(path, options);
}
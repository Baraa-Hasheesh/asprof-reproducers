#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <iostream>
#include <pthread.h>
#include "asprof.h"

#define ASSERT_NO_DLERROR()           \
    err = dlerror();                  \
    if (err != NULL) {                \
        printf("ERROR => %s\n", err); \
        exit(1);                      \
    }

#define ASSERT_NO_ASPROF_ERR(err)                       \
    if (err != NULL) {                                  \
        fprintf(stderr, "%s\n", _asprof_error_str(err)); \
        exit(1);                                        \
    }

void outputCallback(const char* buffer, size_t size) {
    fwrite(buffer, sizeof(char), size, stdout);
}


int main(int argc, char** args) {
    char* err;

    if (argc == 1) {
        void* libprof = dlopen("libasyncProfiler.so", RTLD_NOW);
        ASSERT_NO_DLERROR();

        ((asprof_init_t)dlsym(libprof, "asprof_init"))();
        ASSERT_NO_DLERROR();

        asprof_execute_t _asprof_execute = (asprof_execute_t)dlsym(libprof, "asprof_execute");
        ASSERT_NO_DLERROR();

        asprof_error_str_t _asprof_error_str = (asprof_error_str_t)dlsym(libprof, "asprof_error_str");
        ASSERT_NO_DLERROR();

        asprof_error_t asprof_err = _asprof_execute("start,nativemem,cstack=dwarf,file=output.jfr", outputCallback);
        ASSERT_NO_ASPROF_ERR(asprof_err);

        int* ptr1 = (int*)malloc(1010);

        printf("1. malloc=%d\n", *ptr1);

        asprof_err = _asprof_execute("stop", outputCallback);
        ASSERT_NO_ASPROF_ERR(asprof_err);
    } 

    else {
        int* ptr1 = (int*)malloc(1010);

        printf("2. malloc=%d\n", *ptr1);
    }
}
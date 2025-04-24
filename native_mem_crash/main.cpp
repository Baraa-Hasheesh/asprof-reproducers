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

    
typedef void* (*my_malloc_t)(size_t);
typedef void (*my_free_t)(void*);

void outputCallback(const char* buffer, size_t size) {
    fwrite(buffer, sizeof(char), size, stdout);
}


int main() {
    void* lib;
    char* err;

    lib = dlopen("my_lib.so", RTLD_NOW | RTLD_GLOBAL);
    ASSERT_NO_DLERROR();

    my_malloc_t local_malloc = (my_malloc_t) dlsym(lib, "my_malloc");
    ASSERT_NO_DLERROR();

    my_free_t local_free = (my_free_t) dlsym(lib, "my_free");
    ASSERT_NO_DLERROR();

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

    local_free(local_malloc(1001));

    asprof_err = _asprof_execute("stop", outputCallback);
    ASSERT_NO_ASPROF_ERR(asprof_err);

    dlclose(lib);

    lib = dlopen("my_lib_1.so", RTLD_NOW | RTLD_GLOBAL);
    local_malloc = (my_malloc_t) dlsym(lib, "my_malloc");
    ASSERT_NO_DLERROR();

    local_free = (my_free_t) dlsym(lib, "my_free");
    ASSERT_NO_DLERROR();

    asprof_err = _asprof_execute("start,event=cpu,cstack=dwarf,file=output2.jfr", outputCallback);
    ASSERT_NO_ASPROF_ERR(asprof_err);

    local_free(local_malloc(1010));

    asprof_err = _asprof_execute("stop", NULL);
    ASSERT_NO_ASPROF_ERR(asprof_err);

    dlclose(lib);

    asprof_err = _asprof_execute("start,nativemem,cstack=dwarf,file=output3.jfr", outputCallback);
    ASSERT_NO_ASPROF_ERR(asprof_err);
    asprof_err = _asprof_execute("stop", NULL);
    ASSERT_NO_ASPROF_ERR(asprof_err);
}
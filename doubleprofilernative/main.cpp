#include <asprof.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void* openLib(const char* name) {
    void* ptr = dlopen(name, RTLD_LAZY);
    if (ptr == NULL) {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        exit(1);
    }
    return ptr;
}

void* getSymbol(void* lib, const char* name) {
    void* ptr = dlsym(lib, name);
    if (ptr == NULL) {
        fprintf(stderr, "dlsym error: %s\n", dlerror());
        exit(1);
    }
    return ptr;
}


void outputCallback(const char* buffer, size_t size) {
    fwrite(buffer, sizeof(char), size, stdout);
}

void executeAsyncProfilerCommand(const char* cmd, asprof_execute_t asprof_execute, asprof_error_str_t asprof_error_str) {
    asprof_error_t asprof_err = asprof_execute(cmd, outputCallback);
    if (asprof_err != NULL) {
        fprintf(stderr, "%s\n", asprof_error_str(asprof_err));
        exit(1);
    }
}

void initAsyncProfiler(void* libprof, asprof_execute_t* asprof_execute, asprof_error_str_t* asprof_error_str) {
    asprof_init_t asprof_init = (asprof_init_t)getSymbol(libprof, "asprof_init");
    asprof_init();

    *asprof_execute = (asprof_execute_t)getSymbol(libprof, "asprof_execute");
    *asprof_error_str = (asprof_error_str_t)getSymbol(libprof, "asprof_error_str");
}

int main() {
    printf("Profiler 00\n");
    asprof_execute_t asprof_execute_prof1;
    asprof_error_str_t asprof_error_str_prof1;

    void* handleProf1 = openLib("libasyncProfiler1.so");
    initAsyncProfiler(handleProf1, &asprof_execute_prof1, &asprof_error_str_prof1);
    executeAsyncProfilerCommand("start,event=cpu,file=output1.jfr", asprof_execute_prof1, asprof_error_str_prof1);

    printf("Profiler 1\n");
}
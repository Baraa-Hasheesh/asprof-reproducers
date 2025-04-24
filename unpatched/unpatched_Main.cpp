#include "unpatched_Main.h"
#include <dlfcn.h>
#include <stdlib.h>

typedef void* (*my_dlopen_t)(char*, int);
typedef void* (*my_malloc_t)(size_t);
typedef void (*my_free_t)(void*);

my_dlopen_t local_dlopen = NULL;

JNIEXPORT void JNICALL Java_unpatched_Main_loadLibUnpatched(JNIEnv* jenv, jclass cls) {
    void *ptr = dlopen("libMy_dlopen_lib.so", RTLD_NOW | RTLD_GLOBAL);
    dlclose(ptr);
    ptr = dlopen("libMy_dlopen_lib.so", RTLD_NOW | RTLD_GLOBAL);

    local_dlopen = (my_dlopen_t) dlsym(ptr, "my_dlopen");
}

JNIEXPORT void JNICALL Java_unpatched_Main_execute(JNIEnv* jenv, jclass cls, jstring path) {

    // Convert jstring to char*
    const char* nativePath = jenv->GetStringUTFChars(path, NULL);

    void* lib = local_dlopen((char*)nativePath, RTLD_NOW | RTLD_GLOBAL);

    my_malloc_t local_malloc = (my_malloc_t) dlsym(lib, "my_malloc");
    my_free_t local_free = (my_free_t) dlsym(lib, "my_free");

    local_free(local_malloc(199999999));

    dlclose(lib);

    // IMPORTANT: Release the string when done to prevent memory leaks
    jenv->ReleaseStringUTFChars(path, nativePath);
}
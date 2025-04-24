#ifndef MY_DLOPEN_LIB_H
#define MY_DLOPEN_LIB_H

#define EXPORT __attribute__((visibility("default")))


EXPORT void* my_dlopen(char* path, int options);

#endif
#include <dirent.h>
#include <dlfcn.h>
#include <jni.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "asprof.h"

typedef jint (*CreateJvm)(JavaVM**, void**, void*);

JavaVM* _jvm;
JNIEnv* _env;
void* _jvm_lib;

asprof_error_str_t _asprof_error_str;
asprof_execute_t _asprof_execute;
asprof_init_t _asprof_init;

const char jvm_lib_path[] = "server/libjvm.so";

void* openLib(const char* name) {
    void* ptr = dlopen(name, RTLD_NOW);
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

void loadJvmLib() {
    char* java_home = getenv("JAVA_HOME");
    if (java_home == NULL) {
        fprintf(stderr, "JAVA_HOME is not set\n");
        exit(1);
    }

    // Check that libjvm is found under the standard path
    char lib_path[PATH_MAX + 280];
    snprintf(lib_path, sizeof(lib_path), "%s/%s/%s", java_home, "lib", jvm_lib_path);
    if ((_jvm_lib = dlopen(lib_path, RTLD_NOW)) != NULL) {
        return;
    }

    // JDK 8 has different directory layout. libjvm path will be the following:
    // ${JAVA_HOME}/lib/${ARCH}/server/libjvm.(so|dylib)
    char java_lib_home[PATH_MAX];
    snprintf(java_lib_home, sizeof(java_lib_home), "%s/lib", java_home);

    DIR* dir = opendir(java_lib_home);
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory: %s\n", java_lib_home);
        exit(1);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) {
            continue;
        }

        snprintf(lib_path, sizeof(lib_path), "%s/%s/%s", java_lib_home, entry->d_name, jvm_lib_path);
        if ((_jvm_lib = dlopen(lib_path, RTLD_NOW)) != NULL) {
            break;
        }
    }
    closedir(dir);

    if (_jvm_lib == NULL) {
        fprintf(stderr, "Unable to find: libjvm\n");
        exit(1);
    }
}

void startJvm() {
    JavaVMInitArgs vm_args;
    JavaVMOption options[1];

    options[0].optionString = const_cast<char *>("-Djava.class.path=./");
    // Configure JVM
    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = true;

    CreateJvm createJvm = (CreateJvm)dlsym(_jvm_lib, "JNI_CreateJavaVM");
    if (createJvm == NULL) {
        fprintf(stderr, "Unable to find: JNI_CreateJavaVM\n");
        exit(1);
    }

    // Create the JVM
    jint rc = createJvm(&_jvm, (void**)&_env, &vm_args);
    if (rc != JNI_OK) {
        fprintf(stderr, "Failed to create JVM\n");
        exit(1);
    }
}

void outputCallback(const char* buffer, size_t size) {
    fwrite(buffer, sizeof(char), size, stdout);
}

void executeAsyncProfilerCommand(const char* cmd) {
    asprof_error_t asprof_err = _asprof_execute(cmd, outputCallback);
    if (asprof_err != NULL) {
        fprintf(stderr, "%s\n", _asprof_error_str(asprof_err));
        exit(1);
    }
}

void initAsyncProfiler() {
    _asprof_init = (asprof_init_t)getSymbol(NULL, "asprof_init");
    _asprof_init();

    _asprof_execute = (asprof_execute_t)getSymbol(NULL, "asprof_execute");
    _asprof_error_str = (asprof_error_str_t)getSymbol(NULL, "asprof_error_str");
}

int main() {
    loadJvmLib();
    startJvm();

    initAsyncProfiler();
    executeAsyncProfilerCommand("start,event=ctimer,file=output.jfr");

    jclass customClass = _env->FindClass("Preload");
    if (customClass == nullptr) {
        fprintf(stderr, "Can't find JavaClass\n");
        exit(1);
    }

    jmethodID threadStartTask = _env->GetStaticMethodID(customClass, "threadStart", "()V");
    if (threadStartTask == nullptr) {
        fprintf(stderr, "Can't find threadStart\n");
        exit(1);
    }

    _env->CallStaticVoidMethod(customClass, threadStartTask);
    _env->DeleteLocalRef(customClass);

    executeAsyncProfilerCommand("stop");
}

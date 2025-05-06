#include "thread_Main.h"
#include <pthread.h>
#include <time.h>

void sleepLocal() {
    for (int i = 0; i < 1000000000; i++) {
        for (int j = 0; j < 1; j++) {
                int k = i + j;
        }
    }
    for (int i = 0; i < 1000000000; i++) {
        for (int j = 0; j < 1; j++) {
                int k = i + j;
        }
    }
    for (int i = 0; i < 1000000000; i++) {
        for (int j = 0; j < 1; j++) {
                int k = i + j;
        }
    }
    for (int i = 0; i < 1000000000; i++) {
        for (int j = 0; j < 1; j++) {
                int k = i + j;
        }
    }
}

void* busy(void* arg) {
    pthread_setname_np(pthread_self(), "myNativeThread");

    sleepLocal();

    return NULL;
}

JNIEXPORT void JNICALL Java_thread_Main_startBusyThread(JNIEnv *env, jclass cls) {
    pthread_t native_busy_thread;
    pthread_create(&native_busy_thread, NULL, busy, NULL);
}
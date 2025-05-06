#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

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

void* busy1(void* arg) {
    printf("Hello, World! - myNativeThread1\n");
    pthread_setname_np(pthread_self(), "myNativeThread1");

    sleepLocal();

    return NULL;
}

void* busy2(void* arg) {
    printf("Hello, World! - myNativeThread2\n");
    pthread_setname_np(pthread_self(), "myNativeThread2");

    sleepLocal();

    return NULL;
}

int main() {
    printf("Hello, World!\n");
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, busy1, NULL);
    pthread_create(&thread2, NULL, busy2, NULL);

    sleepLocal();

    printf("End, World!\n");
    return 0;
}

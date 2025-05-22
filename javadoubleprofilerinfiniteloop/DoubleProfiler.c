#include <jni.h>
#include <stdlib.h>

JNIEXPORT void JNICALL Java_javadoubleprofilerinfiniteloop_DoubleProfiler_method(JNIEnv* env, jclass cls) {
    free(malloc(1024 * 1024 * 32));
}

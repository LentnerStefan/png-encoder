#include <jni.h>
#include "react-native-png-encoder.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_pngencoder_PngEncoderModule_initialize(JNIEnv *env, jobject thiz, jlong jsiPtr, jstring cachePath)
{
    auto runtime = reinterpret_cast<facebook::jsi::Runtime*>(jsiPtr);

    const char *cachePathCStr = env->GetStringUTFChars(cachePath, nullptr);

    if (runtime) {
        installPngEncoder(*runtime, cachePathCStr);
    }
    env->ReleaseStringUTFChars(cachePath, cachePathCStr);

}

#include <jni.h>
#include "react-native-png-encoder.h"

extern "C" 
JNIEXPORT void JNICALL
Java_com_pngencoder_PngEncoderModule_initialize(JNIEnv *env, jclass clazz, jlong jsiPtr)
{
    auto runtime = reinterpret_cast<facebook::jsi::Runtime*>(jsiPtr);
    if (runtime) {
      installSequel(*runtime);
    }
}

extern "C" 
JNIEXPORT void JNICALL
Java_com_pngencoder_PngEncoderModule_destruct(JNIEnv *env, jclass clazz)
{
  cleanUpSequel();
}

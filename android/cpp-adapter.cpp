#include <jni.h>
#include "react-native-png-encoder.h"

extern "C"

extern "C" JNIEXPORT void JNICALL
Java_com_pngencoder_PngEncoderModule_initialize(JNIEnv *env, jclass clazz, jlong jsiPtr)
{
  installSequel(*reinterpret_cast<facebook::jsi::Runtime *>(jsiPtr));
}

extern "C" JNIEXPORT void JNICALL
Java_com_pngencoder_PngEncoderModule_destruct(JNIEnv *env, jclass clazz)
{
  cleanUpSequel();
}

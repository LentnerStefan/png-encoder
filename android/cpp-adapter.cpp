#include <jni.h>
#include "react-native-png-encoder.h"

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_pngencoder_PngEncoderModule_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return pngencoder::multiply(a, b);
}

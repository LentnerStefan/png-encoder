#include "react-native-png-encoder.h"
// sstream contains functions to manipulate strings in C++
#include <sstream>
#include <iostream>
#include <jsi/jsi.h>
#include "fpng.h"
#include <random>
#include "MutableRawBuffer.h"
// The namespace allows for syntactic sugar around the JSI objects. ex. call: jsi::Function instead of facebook::jsi::Function
using namespace facebook;

std::string generateRandomFileName() {
    // Generate a random number
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(100000, 999999);

    // Create a file name with the random number
    return "image_" + std::to_string(dist(mt)) + ".png";
}

// We get the runtime from the obj-c code and we create our native functions here
void installPngEncoder(jsi::Runtime& jsiRuntime, const std::string& cachePath) {
    fpng::fpng_init();

auto encode = jsi::Function::createFromHostFunction(
                                                          jsiRuntime, // JSI runtime instance
                                                          jsi::PropNameID::forAscii(jsiRuntime, "encode"), // Internal function name
                                                          3, // Number of arguments in function
                                                          // This is a C++ lambda function, the empty [] at the beginning is used to capture pointer/references so that they don't get de-allocated
                                                          // Then you get another instance of the runtime to use inside the function, a "this" value from the javascript world, a pointer to the arguments (you can treat it as an array) and finally a count for the number of arguments
                                                          // Finally the function needs to return a jsi::Value (read JavaScript value)
                                                          [cachePath](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
                                                              if(!arguments[0].isObject()){
                                                                  throw jsi::JSError(runtime,"Invalid buffer parameter provided");
                                                              }
                                                              if(!arguments[1].isNumber()){
                                                                  throw jsi::JSError(runtime,"Invalid width parameter provided");
                                                              }
                                                              if(!arguments[2].isNumber()){
                                                                  throw jsi::JSError(runtime,"Invalid height parameter provided");
                                                              }
                                                              auto rgbBufferObj = arguments[0].asObject(runtime);
                                                              if(!rgbBufferObj.isArrayBuffer(runtime)){
                                                                  throw jsi::JSError(runtime,"Invalid parameter provided");
                                                              }
                                                              size_t width = arguments[1].asNumber();
                                                              size_t height = arguments[2].asNumber();
                                                              jsi::ArrayBuffer rgbBuffer = rgbBufferObj.getArrayBuffer(runtime);

                                                              uint8_t* rgbData = rgbBuffer.data(runtime);
                                                              size_t dataSize = rgbBuffer.size(runtime);

                                                              // Check the buffer is of the right size
                                                              size_t channelCount = 3;
                                                              if (width * height * channelCount != dataSize) {
                                                                  throw jsi::JSError(runtime, "Invalid RGB buffer size");
                                                              }

                                                              // Generate random file name and create full path
                                                              std::string fileName = generateRandomFileName();
                                                              std::string filePath = cachePath + "/" + fileName;

                                                              // Convert to PNG and save to file
                                                              if (!fpng::fpng_encode_image_to_file(filePath.c_str(), rgbData, width, height, channelCount)) {
                                                                  throw jsi::JSError(runtime, "Failed to encode PNG image to file");
                                                              }

                                                              // Return the file path to JavaScript
                                                              return jsi::String::createFromUtf8(runtime, filePath);
                                                          }
                                                          );
    // Registers the function on the global object
    jsiRuntime.global().setProperty(jsiRuntime, "encode", std::move(encode));
}

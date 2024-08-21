#include "react-native-png-encoder.h"
// sstream contains functions to manipulate strings in C++
#include <sstream>
#include "fpng.h"
#include "fpng.cpp"
#include "MutableRawBuffer.h"

// The namespace allows for syntactic sugar around the JSI objects. ex. call: jsi::Function instead of facebook::jsi::Function
using namespace facebook;

// We get the runtime from the obj-c code and we create our native functions here
void installSequel(jsi::Runtime& jsiRuntime) {
    
    // Initialize fpng once, as stated in the doc: https://github.com/richgel999/fpng?tab=readme-ov-file#initialization
    fpng::fpng_init();
    
    // jsi::Function::createFromHostFunction will create a JavaScript function based on a "host" (read C++) function
    auto multiply = jsi::Function::createFromHostFunction(
                                                          jsiRuntime, // JSI runtime instance
                                                          jsi::PropNameID::forAscii(jsiRuntime, "multiply"), // Internal function name
                                                          3, // Number of arguments in function
                                                          // This is a C++ lambda function, the empty [] at the beginning is used to capture pointer/references so that they don't get de-allocated
                                                          // Then you get another instance of the runtime to use inside the function, a "this" value from the javascript world, a pointer to the arguments (you can treat it as an array) and finally a count for the number of arguments
                                                          // Finally the function needs to return a jsi::Value (read JavaScript value)
                                                          [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
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
                                                              
                                                              
                                                              // Convert to png in memory
                                                              std::vector<uint8_t> pngBuffer;
                                                              fpng::fpng_encode_image_to_memory(rgbData, width, height, channelCount, pngBuffer);
                                                              
                                                              
                                                              // Start conversion to return as an ArrayBuffer on JS Land
                                                              size_t pngSize = pngBuffer.size();
                                                              static constexpr auto ARRAYBUFFER_CACHE_PROP_NAME = "__pngArrayBufferCache";
                                                              
                                                              if (!runtime.global().hasProperty(runtime, ARRAYBUFFER_CACHE_PROP_NAME)) {
                                                                  auto mutableBuffer = std::make_shared<vision::MutableRawBuffer>(pngSize);
                                                                  jsi::ArrayBuffer arrayBuffer(runtime, mutableBuffer);
                                                                  runtime.global().setProperty(runtime, ARRAYBUFFER_CACHE_PROP_NAME, std::move(arrayBuffer));
                                                              }
                                                              
                                                              auto arrayBufferCache = runtime.global().getPropertyAsObject(runtime, ARRAYBUFFER_CACHE_PROP_NAME);
                                                              auto arrayBuffer = arrayBufferCache.getArrayBuffer(runtime);
                                                              
                                                              if (arrayBuffer.size(runtime) != pngSize) {
                                                                  auto mutableBuffer = std::make_shared<vision::MutableRawBuffer>(pngSize);
                                                                  arrayBuffer = jsi::ArrayBuffer(runtime, mutableBuffer);
                                                                  runtime.global().setProperty(runtime, ARRAYBUFFER_CACHE_PROP_NAME, arrayBuffer);
                                                              }
                                                              
                                                              memcpy(arrayBuffer.data(runtime), pngBuffer.data(), pngSize);
                                                              return arrayBuffer;
                                                          }
                                                          );
    // Registers the function on the global object
    jsiRuntime.global().setProperty(jsiRuntime, "multiply", std::move(multiply));
}

void cleanUpSequel() {
    // intentionally left blank
}

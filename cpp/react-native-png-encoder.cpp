#include <chrono>
#include <sstream>
#include <jsi/jsi.h>
#include <filesystem>
#include "react-native-png-encoder.h"
#include "fpng.h"


// The namespace allows for syntactic sugar around the JSI objects. ex. call: jsi::Function instead of facebook::jsi::Function
using namespace facebook;

std::string generateTimestampFileName() {
    // Get the current time as a time_point
    auto now = std::chrono::system_clock::now();
    
    // Convert to milliseconds since epoch
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    // Create a file name using the timestamp
    std::stringstream ss;
    ss << "image_" << ms << ".png";
    return ss.str();
}

// We get the runtime from the obj-c code and we create our native functions here
void installPngEncoder(jsi::Runtime& jsiRuntime, const std::string& cachePath) {
    fpng::fpng_init();
    
    auto saveRgbAsPngFunc = jsi::Function::createFromHostFunction(
                                                                  jsiRuntime, // JSI runtime instance
                                                                  jsi::PropNameID::forAscii(jsiRuntime, "__saveRgbAsPng"), // Internal function name
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
                                                                      std::string fileName = generateTimestampFileName();
                                                                      std::string filePath = cachePath + "/" + fileName;
                                                                      
                                                                      // Convert to PNG and save to file
                                                                      if (!fpng::fpng_encode_image_to_file(filePath.c_str(), rgbData, width, height, channelCount)) {
                                                                          throw jsi::JSError(runtime, "Failed to encode PNG image to file");
                                                                      }
                                                                      
                                                                      // Return the file path to JavaScript
                                                                      return jsi::String::createFromUtf8(runtime, filePath);
                                                                  }
                                                                  );
    
    // Function to delete all files in the cache directory
    auto deleteCacheFilesFunc = jsi::Function::createFromHostFunction(
                                                                      jsiRuntime,
                                                                      jsi::PropNameID::forAscii(jsiRuntime, "__deleteCacheFiles"),
                                                                      0,
                                                                      [cachePath](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
                                                                          namespace fs = std::filesystem;
                                                                          size_t deletedFilesCount = 0;
                                                                          
                                                                          try {
                                                                              for (const auto& entry : fs::directory_iterator(cachePath)) {
                                                                                  if (fs::is_regular_file(entry.path())) {
                                                                                      fs::remove(entry.path());
                                                                                      ++deletedFilesCount;
                                                                                  }
                                                                              }
                                                                          } catch (const fs::filesystem_error& e) {
                                                                              throw jsi::JSError(runtime, "Failed to delete files: " + std::string(e.what()));
                                                                          }
                                                                          
                                                                          // Return the number of deleted files
                                                                          return jsi::Value(static_cast<double>(deletedFilesCount));
                                                                      }
                                                                      );
    
    // Registers the functions on the global object
    jsiRuntime.global().setProperty(jsiRuntime, "__saveRgbAsPng", std::move(saveRgbAsPngFunc));
    jsiRuntime.global().setProperty(jsiRuntime, "__deleteCacheFiles", std::move(deleteCacheFilesFunc));
}

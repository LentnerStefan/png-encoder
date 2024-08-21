#include "react-native-png-encoder.h"
// sstream contains functions to manipulate strings in C++
#include <sstream>
#include "png.h"
#include "MutableRawBuffer.h"

// The namespace allows for syntactic sugar around the JSI objects. ex. call: jsi::Function instead of facebook::jsi::Function
using namespace facebook;

// We get the runtime from the obj-c code and we create our native functions here
void installSequel(jsi::Runtime& jsiRuntime) {
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
        
        if (width * height * 3 != dataSize) {
            throw jsi::JSError(runtime, "Invalid RGB buffer size");
        }
        
        std::vector<uint8_t> pngBuffer;
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr) {
            throw jsi::JSError(runtime, "Failed to create png write struct");
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, nullptr);
            throw jsi::JSError(runtime, "Failed to create png info struct");
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            throw jsi::JSError(runtime, "Error during PNG creation");
        }
        
        auto write_data_fn = [](png_structp png_ptr, png_bytep data, png_size_t length) {
            auto* buffer = static_cast<std::vector<uint8_t>*>(png_get_io_ptr(png_ptr));
            buffer->insert(buffer->end(), data, data + length);
        };

        png_set_write_fn(png_ptr, &pngBuffer, write_data_fn, nullptr);
        
        // Set PNG compression level to prioritize speed
        png_set_compression_level(png_ptr, 0);
        
        // Disable all PNG filters for speed
        png_set_filter(png_ptr, 0, PNG_FILTER_NONE);

        // Optional: Set compression buffer size (can adjust this as needed)
        png_set_compression_buffer_size(png_ptr, 8192);
        
        png_set_IHDR(
            png_ptr,
            info_ptr,
            width,
            height,
            8, // bit depth
            PNG_COLOR_TYPE_RGB, // color type
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
        );
        
        std::vector<png_bytep> row_pointers(height);
        for (size_t y = 0; y < height; ++y) {
            row_pointers[y] = rgbData + y * width * 3;
        }

        png_write_info(png_ptr, info_ptr);
        png_write_image(png_ptr, row_pointers.data());
        png_write_end(png_ptr, nullptr);

        png_destroy_write_struct(&png_ptr, &info_ptr);
        
        // Convert pngBuffer to a jsi::ArrayBuffer
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

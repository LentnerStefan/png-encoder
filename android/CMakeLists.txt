cmake_minimum_required(VERSION 3.4.1)
project(PngEncoder)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)

# set(ZXING_READERS ON)
# set(ZXING_WRITERS OFF)

add_subdirectory("../zxing-cpp/core" ZXing EXCLUDE_FROM_ALL)


add_library(pngencoder            SHARED
            "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi/jsi/jsi.cpp"
            "../cpp/react-native-png-encoder.cpp"
            "../cpp/fpng.cpp"
            "cpp-adapter.cpp"
)

# Specifies a path to native header files.
include_directories(
            ../cpp
            "${NODE_MODULES_DIR}/react-native/React"
            "${NODE_MODULES_DIR}/react-native/React/Base"
            "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi"
            "../zxing-cpp/core/src"
)

target_link_libraries(pngencoder PRIVATE ZXing::ZXing)

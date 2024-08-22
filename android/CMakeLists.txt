cmake_minimum_required(VERSION 3.4.1)
project(PngEncoder)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 14)

add_library(pngencoder            SHARED
            ../node_modules/react-native/ReactCommon/jsi/jsi/jsi.cpp
            ../cpp/react-native-png-encoder.cpp
            ../cpp/fpng.cpp
            cpp-adapter.cpp
)

# Specifies a path to native header files.
include_directories(
            ../cpp
            ../node_modules/react-native/React
            ../node_modules/react-native/React/Base
            ../node_modules/react-native/ReactCommon/jsi
)

target_link_libraries(pngencoder)
cmake_minimum_required(VERSION 3.10.2)

project(reactnativezstd CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)

#
set(ZSTD_BUILD_PROGRAMS OFF) # Avoid  Target "zstd" of type EXECUTABLE may not be linked into another target error
set(ZSTD_LEGACY_SUPPORT OFF)
set(ZSTD_BUILD_TESTS OFF)
#set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(../externals/zstd/build/cmake _zstd)

#
# find_package(fbjni REQUIRED CONFIG)

add_library(reactnativezstd
        SHARED
        ./cpp-adapter.cpp
        ../cpp/react-native-zstd.cpp
        ../cpp/jsi-utils/JSIUtils.cpp
        ../cpp/ZstdHostObject.cpp
        # To avoid: undefined reference to 'facebook::jsi::Value::~Value()'
        # ../node_modules/react-native/ReactCommon/jsi/jsi/jsi.cpp
        )

target_include_directories(
        reactnativezstd
        PUBLIC
        ../externals/zstd/lib
        ../cpp
        ../node_modules/react-native/ReactCommon/jsi
)


target_link_libraries(
        reactnativezstd
        PRIVATE
        libzstd_static
        # fbjni::fbjni
)

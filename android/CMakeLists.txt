cmake_minimum_required(VERSION 3.10.2)

project(reactnativezstd)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
#
set(ZSTD_BUILD_PROGRAMS OFF) # Avoid  Target "zstd" of type EXECUTABLE may not be linked into another target error
set(ZSTD_LEGACY_SUPPORT OFF)
set(ZSTD_BUILD_TESTS OFF)

add_subdirectory(../externals/zstd/build/cmake _zstd)
#

add_library(reactnativezstd
        SHARED
        ../cpp/react-native-zstd.cpp
        ./cpp-adapter.cpp
        )

target_include_directories(
        reactnativezstd
        PUBLIC
        ../externals/zstd/lib
        ../cpp
)

target_link_libraries(
        reactnativezstd
        PRIVATE
        libzstd_static
)

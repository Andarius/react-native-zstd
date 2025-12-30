project(reactnativezstd)
cmake_minimum_required(VERSION 3.9.0)

set(PACKAGE_NAME reactnativezstd)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)

# ZSTD configuration
set(ZSTD_BUILD_PROGRAMS OFF)
set(ZSTD_LEGACY_SUPPORT OFF)
set(ZSTD_BUILD_TESTS OFF)
set(ZSTD_BUILD_STATIC ON)
set(ZSTD_BUILD_SHARED OFF)

add_subdirectory(../externals/zstd/build/cmake _zstd)

# Define C++ library and add all sources
add_library(${PACKAGE_NAME} SHARED
    src/main/cpp/cpp-adapter.cpp
    ../cpp/react-native-zstd.cpp
    ../cpp/HybridZstd.cpp
)

# Add Nitrogen specs
include(${CMAKE_SOURCE_DIR}/../nitrogen/generated/android/reactnativezstd+autolinking.cmake)

# Set up local includes
target_include_directories(
    ${PACKAGE_NAME}
    PUBLIC
    ../externals/zstd/lib
    ../cpp
    "src/main/cpp"
)

find_library(LOG_LIB log)

# Link all libraries together
target_link_libraries(
    ${PACKAGE_NAME}
    ${LOG_LIB}
    android
    libzstd_static
)

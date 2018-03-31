# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBORINGSSL_SHARED_LIBRARY")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGOOGLE_PROTOBUF_NO_RTTI")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGOOGLE_PROTOBUF_NO_STATIC_INITIALIZER")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPROTOBUF_USE_DLLS")
add_library( chromium_base
             SHARED
             IMPORTED )

set_target_properties( chromium_base
                       PROPERTIES IMPORTED_LOCATION
                       ${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/libs/${ANDROID_ABI}/libbase.cr.so )

add_library( chromium_url
             SHARED
             IMPORTED )

set_target_properties( chromium_url
                       PROPERTIES IMPORTED_LOCATION
                       ${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/libs/${ANDROID_ABI}/liburl.cr.so )

add_library( chromium_net
             SHARED
             IMPORTED )

set_target_properties( chromium_net
                       PROPERTIES IMPORTED_LOCATION
                       ${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/libs/${ANDROID_ABI}/libnet.cr.so )

add_library( neteasenet
             SHARED
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/NetJni.cpp )


target_include_directories(neteasenet PRIVATE
                           ${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/include )

target_link_libraries( neteasenet
                       chromium_net
                       chromium_base
                       chromium_url
                       android
                       log
                       z)

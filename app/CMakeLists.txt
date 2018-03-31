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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

add_library( chromium_base
             SHARED
             IMPORTED )
set_target_properties( # Specifies the target library.
                       chromium_base

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/libs/${ANDROID_ABI}/libbase.cr.so )
add_library( chromium_url
             SHARED
             IMPORTED )

set_target_properties( # Specifies the target library.
                       chromium_url

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/libs/${ANDROID_ABI}/liburl.cr.so )

add_library( chromium_net
             SHARED
             IMPORTED )

set_target_properties( # Specifies the target library.
                       chromium_net

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/libs/${ANDROID_ABI}/libnet.cr.so )

add_library( # Sets the name of the library.
             neteasenet

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/jni/src/NetJni.cpp )


target_include_directories(neteasenet PRIVATE
                           ${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/include )

target_link_libraries( neteasenet
                       chromium_net
                       chromium_base
                       chromium_url
                       android
                       log
                       z)
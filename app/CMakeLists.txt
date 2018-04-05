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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti -fno-threadsafe-statics -Wno-narrowing -Wno-char-subscripts")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -latomic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBORINGSSL_SHARED_LIBRARY")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGOOGLE_PROTOBUF_NO_RTTI")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGOOGLE_PROTOBUF_NO_STATIC_INITIALIZER")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPROTOBUF_USE_DLLS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DV8_DEPRECATION_WARNINGS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_NOTIFICATIONS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_BROWSER_CDMS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_PRINTING=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_BASIC_PRINTING=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_SPELLCHECK=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_BROWSER_SPELLCHECKER=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_OPENSSL_CERTS=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_TCMALLOC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_EXTERNAL_POPUP_MENU=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_NACL")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_SUPERVISED_USERS=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCHROMIUM_BUILD")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDANDROID")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_SYS_UIO_H")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STDC_CONSTANT_MACROS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STDC_FORMAT_MACROS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMPONENT_BUILD")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__GNU_SOURCE=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDYNAMIC_ANNOTATIONS_ENABLED=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWTF_USE_DYNAMIC_ANNOTATIONS=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDLOPEN_KERBEROS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNET_IMPLEMENTATION")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_KERBEROS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_BUILT_IN_DNS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPOSIX_AVOID_MMAP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_WEBSOCKETS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_PTHREAD")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DU_USING_ICU_NAMESPACE=0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DU_ENABLE_DYLOAD=0")

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


add_library( boring_ssl
             SHARED
             IMPORTED )

set_target_properties( boring_ssl
                       PROPERTIES IMPORTED_LOCATION
                       ${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/libs/${ANDROID_ABI}/libboringssl.cr.so )


add_library( neteasenet
             SHARED
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/NetJni.cpp
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/chlo_extractor.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_client_base.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_client_message_loop_network_helper.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_dispatcher.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_http_response_cache.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_per_connection_packet_writer.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_simple_client.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_simple_crypto_server_stream_helper.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_simple_dispatcher.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_simple_per_connection_packet_writer.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_simple_server.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_simple_server_packet_writer.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_simple_server_session.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_simple_server_session_helper.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_simple_server_stream.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_spdy_client_base.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_spdy_client_session.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_spdy_client_stream.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_spdy_server_stream_base.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/quic_time_wait_list_manager.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/stateless_rejector.cc
             ${PROJECT_SOURCE_DIR}/src/main/jni/src/tools/quic/synchronous_host_resolver.cc)

include_directories(${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/boringssl/src/include)

include_directories(${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/protobuf/)


target_include_directories( neteasenet PUBLIC
                           ${PROJECT_SOURCE_DIR}/src/main/jni/third_party/chromium/include )

target_link_libraries( neteasenet
                       chromium_net
                       chromium_base
                       chromium_url
                       boring_ssl
                       android
                       log
                       z)

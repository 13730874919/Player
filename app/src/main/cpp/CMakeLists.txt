# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#ffmpeg路径
set(FFMPEG_DIR "D:/AndroidWork/ffmpeg/app")

#set(FFMPEG_DIR "../../../../")
#ffmpeg头文件路径
set(FFMPEG_INCLUDE_DIR ${FFMPEG_DIR}/include)
set(FFMPEG_PACK_INCLUDE_DIR ${FFMPEG_DIR}/src/main/cpp/FFmpeg)
set(IPLAYER_INCLUDE_DIR ${FFMPEG_DIR}/src/main/cpp/IPlayer)
set(TOOL_INCLUDE_DIR ${FFMPEG_DIR}/src/main/cpp/Tool)
set(OPENGL_INCLUDE_DIR ${FFMPEG_DIR}/src/main/cpp/OpenGL)
#ffmpeg库路径
set(FFMPEG_LIB_DIR ${FFMPEG_DIR}/libs/${ANDROID_ABI})


include_directories(include ${FFMPEG_INCLUDE_DIR})
include_directories(include ${FFMPEG_PACK_INCLUDE_DIR})
include_directories(include ${IPLAYER_INCLUDE_DIR})
include_directories(include ${TOOL_INCLUDE_DIR})
include_directories(include ${OPENGL_INCLUDE_DIR})

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavcodec.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavutil.so)




add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswscale.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswresample.so)


add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED
            Tool/XLog.cpp
            Tool/XData.cpp
            FFmpeg/FFDemux.cpp
            FFmpeg/FFDecode.cpp
        FFmpeg/FFResample.cpp
            IPlayer/IDemux.cpp
            IPlayer/IDecode.cpp
        IPlayer/IResample.cpp
        IPlayer/IAudioPlay.cpp
            IPlayer/IVideoView.cpp
            Tool/XThread.cpp
            Tool/XParameter.cpp
            Tool/IObserver.cpp
            OpenGL/XEGL.cpp
            OpenGL/XShader.cpp
        OpenGL/SLAudioPlay.cpp
            OpenGL/XTexture.cpp
            OpenGL/GLVideoView.cpp
        OpenGL/SLAudioPlay.cpp
             # Provides a relative path to your source file(s).
             native-lib.cpp )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib
GLESv2 EGL
OpenSLES
android
avcodec avformat avutil swscale swresample
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
cmake_minimum_required(VERSION 3.10)
project(AudioPlayer)

set(CMAKE_CXX_STANDARD 20)

# Пути к Homebrew FFmpeg
set(FFMPEG_INCLUDE_DIR "/opt/homebrew/opt/ffmpeg/include")
set(FFMPEG_LIB_DIR "/opt/homebrew/opt/ffmpeg/lib")

# Заголовки
include_directories(${FFMPEG_INCLUDE_DIR})

# Исходник
add_executable(AudioPlayer core/main.cpp
        core/PlayAudio.h
        core/PlayAudio.cpp)

# AudioToolbox - фреймворк, который внутри себя содержит и .h .dylib поэтому не нужно отдельно подключать заголовки
find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox) # поиск библиотеки с названием AudioToolbox

# Линкуем напрямую на .dylib
target_link_libraries(AudioPlayer
        "${FFMPEG_LIB_DIR}/libavcodec.dylib"
        "${FFMPEG_LIB_DIR}/libavformat.dylib"
        "${FFMPEG_LIB_DIR}/libavutil.dylib"
        "${FFMPEG_LIB_DIR}/libswresample.dylib"
        ${AUDIOTOOLBOX_LIBRARY}
)

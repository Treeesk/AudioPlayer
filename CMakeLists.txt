cmake_minimum_required(VERSION 3.10)
project(AudioPlayer)

set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.16)

project(AudioPlayerQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

# Пути к Homebrew FFmpeg
set(FFMPEG_INCLUDE_DIR "/opt/homebrew/opt/ffmpeg/include")
set(FFMPEG_LIB_DIR "/opt/homebrew/opt/ffmpeg/lib")

# Заголовки
include_directories(${FFMPEG_INCLUDE_DIR})

# Исходник
add_executable(AudioPlayer core/main.cpp
        core/PlayAudio.h
        core/PlayAudio.cpp
        core/Errors.h
        core/AudioFileLoader.h
        core/AudioFileLoader.cpp
        resources.qrc
)
# Пути к TagLib
set(TAGLIB_INCLUDE_DIR "/opt/homebrew/include/taglib")
set(TAGLIB_LIB_DIR "/opt/homebrew/lib")

# Заголовки
include_directories(${FFMPEG_INCLUDE_DIR})
include_directories(${TAGLIB_INCLUDE_DIR})

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

# Создать библиотеку из core
add_library(core_lib STATIC
    core/Source/AudioFileLoader.cpp
    core/Source/PlayAudio.cpp
    core/Headers/AudioFileLoader.h
    core/Headers/Errors.h
    core/Headers/PlayAudio.h
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AudioPlayerQt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        track.h
        track.cpp
        TrackInfoUI.h
        TrackInfoUI.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET AudioPlayerQt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(AudioPlayerQt SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AudioPlayerQt
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# линковка и подключение header файлов и библиотек к core_lib и соответ. к audioplayerqt
target_include_directories(core_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core/headers
    ${FFMPEG_INCLUDE_DIR}
)
target_link_libraries(core_lib PUBLIC
    "${FFMPEG_LIB_DIR}/libavcodec.dylib"
    "${FFMPEG_LIB_DIR}/libavformat.dylib"
    "${FFMPEG_LIB_DIR}/libavutil.dylib"
    "${FFMPEG_LIB_DIR}/libswresample.dylib"
    ${AUDIOTOOLBOX_LIBRARY}
)
target_link_libraries(core_lib
    PRIVATE
        ${TAGLIB_LIB_DIR}/libtag.dylib
)

# Линковка движка к приложению
target_link_libraries(AudioPlayerQt PRIVATE core_lib)

target_link_libraries(AudioPlayerQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AudioPlayerQt)
endif()
set_target_properties(AudioPlayerQt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AudioPlayerQt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AudioPlayerQt)
endif()

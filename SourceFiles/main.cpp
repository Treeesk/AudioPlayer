#include "mainwindow.h"
#include <QApplication>
#include "PlayAudio.h"
#include "AudioFileLoader.h"
#include <filesystem>
#include <deque>
#include <iostream>
#include "track.h"

int main(int argc, char *argv[])
{
    const char* path_to_dir = "/Users/yarchick/AudioPlayerQt/testingmp3mus";
    std::deque<std::filesystem::path> paths_to_files = GetAudioFiles(path_to_dir);
    QVector<track> tracks;
    tracks.reserve(paths_to_files.size());
    while (!paths_to_files.empty()) {
        try {
            tracks.push_back(track(paths_to_files.front().c_str()));
        }
        catch (const std::exception& e){
            std::cerr << e.what() << std::endl;
        }
        paths_to_files.pop_front();
    }
    // while (!paths_to_files.empty()) {

    //     try {
    //         PlayAudio(path);
    //     } catch (const std::exception& e) {
    //         std::cerr << e.what() << std::endl;
    //     }
    //     paths_to_files.pop_front();
    // }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

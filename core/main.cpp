#include "PlayAudio.h"
#include "AudioFileLoader.h"
#include <iostream>
#include <deque>
#include <filesystem>
#include <algorithm>
int main() {
    const char* path_to_dir = "../testingmp3mus";
    std::deque<std::filesystem::path> paths_to_files = GetAudioFiles(path_to_dir);
    try {
        while (!paths_to_files.empty()) {
            const char* path = paths_to_files.front().c_str();
            PlayAudio(path);
            paths_to_files.pop_front();
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
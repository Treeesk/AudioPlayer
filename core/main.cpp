#include "PlayAudio.h"
#include <iostream>
#include <deque>
#include <filesystem>
#include <algorithm>
int main() {
    std::deque<std::filesystem::path> paths_to_files;
    const char* path_to_dir = "../testingmp3mus";
    for (auto const& file: std::filesystem::directory_iterator(path_to_dir)) {
        auto filename = file.path().filename().string();
        if (!filename.empty() && filename[0] == '.') {
            continue;
        }
        paths_to_files.push_back(file.path());
    }
    std::sort(paths_to_files.begin(), paths_to_files.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) {
        return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b);
    });
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
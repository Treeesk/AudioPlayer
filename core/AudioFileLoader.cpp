#include "AudioFileLoader.h"
#include "iostream"

std::deque<std::filesystem::path> GetAudioFiles(const char* path_to_dir) {
    std::deque<std::filesystem::path> paths_to_files;
    for (auto const& file: std::filesystem::directory_iterator(path_to_dir)) {
        auto filename = file.path().filename().string();
        if (!filename.empty() && filename[0] == '.') { // skip hidden work files
            continue;
        }
        paths_to_files.push_back(file.path());
    }
    std::sort(paths_to_files.begin(), paths_to_files.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) {
        return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b);
    });
    return paths_to_files;
}
#include "DataManager.h"
#include <iostream>
#include "AudioFileLoader.h"
#include "PlayAudio.h"

void MusicDataManager::loadfromdata(const char* path_to_dir) {
    std::deque<std::filesystem::path> paths_to_files = GetAudioFiles(path_to_dir);
    tracks.reserve(paths_to_files.size());
    _currenttrackind = paths_to_files.size() != 0 ? 0 : -1;
    while (!paths_to_files.empty()) {
        try {
            tracks.push_back(track(paths_to_files.front().c_str()));
        }
        catch (const std::exception& e){
            std::cerr << e.what() << std::endl;
        }
        paths_to_files.pop_front();
    }
}

const track& MusicDataManager::currenttrack() {
    return tracks[_currenttrackind];
}

const QVector<track>& MusicDataManager::alltracks() {
    return tracks;
}

void MusicDataManager::play() {
    _isplaying = true;
    if (!launchtrack) {
        launchtrack = true;
        ResetPlay();
        PlayAudio(tracks[_currenttrackind].getpath().c_str());
    }
    else {
        ResumePlay();
    }
}

void MusicDataManager::pause() {
    _isplaying = false;
    Pause();
}

void MusicDataManager::next() {
    launchtrack = false;
    if (_currenttrackind == tracks.size() - 1) {
        _currenttrackind = 0;
    }
    else {
        _currenttrackind++;
    }
        // emit на Сигнал currenttrackchange, чтобы изменить внешний вид
    play();
    // emit на кнопку, которая взаимодействует с PLAY/STOP audio. Чтобы кнопка переключилась
}

void MusicDataManager::prev() {
    launchtrack = false;
    if (_currenttrackind == 0) {
        _currenttrackind = tracks.size() - 1;
    }
    else {
        _currenttrackind--;
    }
        // emit на Сигнал currenttrackchange, чтобы изменить внешний вид
    play();
    // emit на кнопку, которая взаимодействует с PLAY/STOP audio. Чтобы кнопка переключилась
}

#include "DataManager.h"
#include <iostream>
#include "AudioFileLoader.h"
#include "UI/TrackInfoUI.h"

MusicDataManager::MusicDataManager(QObject* parent): QObject(parent) {
    worker = new AudioWorker();
    worker->moveToThread(&audioThread);
    connect(this, &MusicDataManager::playRequested, worker, &AudioWorker::playTrack, Qt::QueuedConnection);
    connect(this, &MusicDataManager::resumeRequested, worker, &AudioWorker::ResumeTrack, Qt::QueuedConnection);
    connect(this, &MusicDataManager::pauseRequested, worker, &AudioWorker::pauseTrack, Qt::QueuedConnection);
    connect(this, &MusicDataManager::seekRequested, worker, &AudioWorker::seekAudio, Qt::QueuedConnection);
    connect(this, &MusicDataManager::NewVolRequested, worker, &AudioWorker::newVolume, Qt::QueuedConnection);
    connect(this, &MusicDataManager::resetEngine, worker, &AudioWorker::ResetEngine, Qt::QueuedConnection); // для перезагрузки движка, чтобы остановить прошлое воспроизведение
    audioThread.start();
}

MusicDataManager::~MusicDataManager() {
    audioThread.quit();
    audioThread.wait();
}

void MusicDataManager::loadfromdata(const char* path_to_dir) {
    emit resetEngine();
    emit resetPCW();
    tracks.clear();
    _isplaying = false;
    launchtrack = false;
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
    if (!tracks.empty()) {
        emit setTrackfromDir(currenttrack());
        emit setTracksfromDir(alltracks());
        emit tracksLoaded();
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
        emit playRequested(tracks[_currenttrackind].getpath().c_str());
    }
    else {
        emit resumeRequested();
    }
}

void MusicDataManager::pause() {
    _isplaying = false;
    emit pauseRequested();
}

void MusicDataManager::next() {
    launchtrack = false;
    if (_currenttrackind == tracks.size() - 1) {
        _currenttrackind = 0;
    }
    else {
        _currenttrackind++;
    }
    emit currenttrackchange(currenttrack());
    emit currentTrackIndexChanged(_currenttrackind);
    play();
}

void MusicDataManager::prev() {
    launchtrack = false;
    if (_currenttrackind == 0) {
        _currenttrackind = tracks.size() - 1;
    }
    else {
        _currenttrackind--;
    }
    emit currenttrackchange(currenttrack());
    emit currentTrackIndexChanged(_currenttrackind);
    play();
}

void MusicDataManager::PanelChangeTrack(int index) {
    launchtrack = false;
    _currenttrackind = index;
    emit currenttrackchange(currenttrack());
    play();
}

void MusicDataManager::seekingAudio(int value) {
    _isplaying = true;
    launchtrack = true;
    emit seekRequested(value, tracks[_currenttrackind].getpath().c_str());
}

void MusicDataManager::setVolume(int value) {
    emit NewVolRequested(value);
}

double GetDuration(const char* path){
    return Player::GetDurationWithFFprobe(path);
}

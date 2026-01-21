#include "AudioWrapper.h"
#include "PlayAudio.h"
void AudioWorker::seekAudio(int value, const char* path) {
    player->SeekAudio(value, path);
}

void AudioWorker::pauseTrack() {
    player->Pause();
}

void AudioWorker::ResumeTrack() {
    player->ResumePlay();
}

void AudioWorker::playTrack(const char* path) {
    player->ResetPlay();
    player->PlayAudio(path);
}

void AudioWorker::newVolume(int value) {
    player->changeVol(value);
}

void AudioWorker::ResetEngine() {
    player->ResetPlay();
}

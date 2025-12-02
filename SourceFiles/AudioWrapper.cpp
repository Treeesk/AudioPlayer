#include "AudioWrapper.h"
#include "PlayAudio.h"

void AudioWorker::seekAudio(int value) {
    // вызов функции из движка с перемещением трека
    player->SeekAudio(value);
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

#ifndef PLAYAUDIO_H
#define PLAYAUDIO_H
#include "AudioToolbox/AudioToolbox.h"

void PlayAudio(const char *fp);
void PlayPCM(AudioQueueRef& que);
void AudioCallbackF(void* UD, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);
void averr_process(int err);
void Pause();
void ResumePlay();
void ResetPlay();
double GetDurationWithFFprobe(const char* filename);
#endif

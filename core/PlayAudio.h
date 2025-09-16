#ifndef PLAYAUDIO_H
#define PLAYAUDIO_H
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <mutex>
#include "AudioToolbox/AudioToolbox.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
}

int PlayAudio(const char *fp);
void PlayPCM(AudioQueueRef& que);
void AudioCallbackF(void* UD, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);

#endif

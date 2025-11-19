#ifndef PLAYAUDIO_H
#define PLAYAUDIO_H
#include "AudioToolbox/AudioToolbox.h"
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
}

class Player {
private:
    std::queue<std::vector<uint8_t>> que;
    std::mutex mtx;
    std::atomic_bool isplaying;
    std::condition_variable pause_cv;
    std::mutex pause_mtx;
    AudioQueueRef queue;
    AVFormatContext* fmt_ctx;
    AVStream* audio_stream;
    AVCodecContext* codec_ctx; // будет храниться состояние декодера(состояние потока, внутренние буфера и тд)
    struct AudioContext {
        std::queue<std::vector<uint8_t>>* que;
        std::mutex* mtx;
    };
    void PlayPCM();
    static void AudioCallbackF(void* UD, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);
    void averr_process(int err);
public:
    Player() {}
    void PlayAudio(const char *fp);
    void Pause();
    void ResumePlay();
    void ResetPlay();
    static double GetDurationWithFFprobe(const char* filename);
};

#endif

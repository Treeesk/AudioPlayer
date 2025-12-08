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
    std::atomic<double> volume = 1.0;
    std::atomic<bool> need_seek = false;
    std::atomic<int> seek_target_time;
    std::queue<std::vector<uint8_t>> que;
    std::mutex mtx;
    std::atomic_bool isplaying;
    std::condition_variable pause_cv;
    std::mutex pause_mtx;
    AudioQueueRef queue;
    AVFormatContext* fmt_ctx;
    AVStream* audio_stream;
    SwrContext* swr_context;
    int stream_index;
    AVCodecContext* codec_ctx; // будет храниться состояние декодера(состояние потока, внутренние буфера и тд)
    struct AudioContext {
        std::queue<std::vector<uint8_t>>* que;
        std::mutex* mtx;
    };
    void PlayPCM();
    static void AudioCallbackF(void* UD, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);
    void averr_process(int err);

    const char* current_path;
public:
    Player() {}
    void PlayAudio(const char *fp);
    void Pause();
    void ResumePlay();
    void ResetPlay();
    void SeekAudio(int target_time, const char* path);
    void changeVol(int vol);
    static double GetDurationWithFFprobe(const char* filename);
};

#endif

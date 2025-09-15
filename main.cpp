#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include "AudioToolbox/AudioToolbox.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
}

#include <mutex>

std::queue<std::vector<uint8_t>> que;
std::mutex mtx;

void AudioCallbackF(void* UserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer) {
    std::lock_guard<std::mutex> lock(mtx);
    if (que.empty()) {
        memset(inBuffer->mAudioData, 0, inBuffer->mAudioDataBytesCapacity);
        inBuffer->mAudioDataByteSize = inBuffer->mAudioDataBytesCapacity;
    } else {
        auto& front = que.front();
        size_t bytes_copy = front.size();
        memcpy(inBuffer->mAudioData, front.data(), bytes_copy);
        inBuffer->mAudioDataByteSize = bytes_copy;
        que.pop();
    }
    AudioQueueEnqueueBuffer(inAQ, inBuffer, 0, NULL);
}

void PlayPcm(AudioQueueRef& queue) {
    AudioStreamBasicDescription format = {0};
    format.mSampleRate = 44100;
    format.mFormatID = kAudioFormatLinearPCM;
    format.mChannelsPerFrame = 2;
    format.mBitsPerChannel = 16;
    format.mFramesPerPacket = 1;
    format.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
    format.mBytesPerFrame = 4;
    format.mBytesPerPacket = 4;

    AudioQueueNewOutput(&format, AudioCallbackF, nullptr, NULL, NULL, 0, &queue);

    for (int i = 0; i < 3; ++i) {
        AudioQueueBufferRef buffer;
        AudioQueueAllocateBuffer(queue, 4096, &buffer);
        memset(buffer->mAudioData, 0, buffer->mAudioDataBytesCapacity);
        buffer->mAudioDataByteSize = buffer->mAudioDataBytesCapacity;
        AudioQueueEnqueueBuffer(queue, buffer, 0, NULL);
    }
    AudioQueueStart(queue, nullptr);
}

int main() {
    const char* path = "/Applications/AudioPlayer/testingmp3mus/ANNA_ASTI_-_Po_baram_74376135.mp3";

    AVFormatContext* fmt_ctx = nullptr;
    if (avformat_open_input(&fmt_ctx, path, NULL, NULL) != 0) {
        std::cerr << "Could not open AVFormat stream" << std::endl;
        // добавить исключение вылетающее
    }
    // получение подробной информации потоков(битрейт, частоту и тд.)
    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        std::cerr << "Не удалось найти stream info" << std::endl;
        return -1;
    }
    // поиск лучшего потока нужен, тк. необязательно у нас есть только аудио дорожка
    int stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (stream_index < 0) {
        std::cerr << "Аудиопоток не найден" << std::endl;
        return -1;
    }
    // указатель на выбранный аудио поток
    AVStream* audio_stream = fmt_ctx->streams[stream_index];

    // Получаем декодер
    const AVCodec* codec = avcodec_find_decoder(audio_stream->codecpar->codec_id);
    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec); // будет храниться состояние декодера(состояние потока, внутренние буфера и тд)
    avcodec_parameters_to_context(codec_ctx, audio_stream->codecpar); // копирование параметров в контекст декодера
    avcodec_open2(codec_ctx, codec, nullptr);

    // audio resampling parametres (чтобы приводить все файлы к одному виду)
    SwrContext* swr_context = nullptr;
    AVChannelLayout channel_layout;
    av_channel_layout_default(&channel_layout, 2); // stereo
    swr_alloc_set_opts2(
        &swr_context,
        &channel_layout,
        AV_SAMPLE_FMT_S16,
        44100,
        &codec_ctx->ch_layout,
        codec_ctx->sample_fmt,
        codec_ctx->sample_rate,
        0, nullptr);
    swr_init(swr_context);

    // создание буферов
    AVPacket * pkt = av_packet_alloc(); // structure stores compressed data
    AVFrame* frame = av_frame_alloc(); // structure describes decoded (raw) audio data

    AudioQueueRef queue;
    PlayPcm(queue); // запускаем сразу

    // цикл чтения файла
    while (!av_read_frame(fmt_ctx, pkt)) {
        if (pkt->stream_index == stream_index) {
            avcodec_send_packet(codec_ctx, pkt);
            while (!avcodec_receive_frame(codec_ctx, frame)) {
                uint8_t* out_data;
                int out_linesize;
                av_samples_alloc(&out_data, &out_linesize, 2, frame->nb_samples, AV_SAMPLE_FMT_S16, 0);

                int samples = swr_convert(swr_context, &out_data, frame->nb_samples, (const uint8_t**)frame->data, frame->nb_samples);
                int buffer_size = samples * 2 * 2; // cnt_samples * channels * byte_per_sample
                // разрезать на чанки по 4096 байт, т.к один буффер в queue будет содержать максимум 4096 байт
                int chunk_size = 4096;
                for (int offset = 0; offset < buffer_size; offset += chunk_size) {
                    int copy_size = std::min(chunk_size, buffer_size - offset);
                    std::vector<uint8_t> pcm(copy_size);
                    memcpy(pcm.data(), out_data + offset, copy_size);
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        que.push(std::move(pcm));
                    }
                }
                av_freep(&out_data);
            }
        }
        av_packet_unref(pkt);
    }

    while (!que.empty()); // нужно не завершать работу main, чтобы mtx не пропал и Core Audio мог работать

    AudioQueueStop(queue, false); // остановка и очистка структур, потенциально может пригодится, если будет много одновременно вызовов
    AudioQueueDispose(queue, true);
    // free memory
    av_frame_free(&frame);
    av_packet_free(&pkt);
    swr_free(&swr_context);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);
    return 0;
}
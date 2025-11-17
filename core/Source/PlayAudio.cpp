#include "PlayAudio.h"
#include "Errors.h"
#include <string>
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

std::queue<std::vector<uint8_t>> que;
std::mutex mtx;
std::atomic_bool isplaying;
std::condition_variable pause_cv;
std::mutex pause_mtx;
AudioQueueRef queue;

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
    AudioQueueEnqueueBuffer(inAQ, inBuffer, 0, NULL); // передача pcm из que в queue
}

void PlayPcm() {
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

void PlayAudio(const char* path) {
    isplaying = true;
    AVFormatContext* fmt_ctx = nullptr;
    int check_err;
    check_err = avformat_open_input(&fmt_ctx, path, NULL, NULL);
    averr_process(check_err);
    // получение подробной информации потоков(битрейт, частоту и тд.)
    check_err = avformat_find_stream_info(fmt_ctx, 0);
    averr_process(check_err);
    // поиск лучшего потока нужен, тк. необязательно у нас есть только аудио дорожка
    int stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    averr_process(stream_index);
    // указатель на выбранный аудио поток
    AVStream* audio_stream = fmt_ctx->streams[stream_index];

    // Получаем декодер
    const AVCodec* codec;
    if ((codec = avcodec_find_decoder(audio_stream->codecpar->codec_id)) == nullptr) {
        throw PlayAudioErr("Could not find codec");
    }
    AVCodecContext* codec_ctx; // будет храниться состояние декодера(состояние потока, внутренние буфера и тд)
    if ((codec_ctx = avcodec_alloc_context3(codec)) == nullptr) {
        throw PlayAudioErr("Could not allocate audio codec context");
    }
    // копирование параметров в контекст декодера
    check_err = avcodec_parameters_to_context(codec_ctx, audio_stream->codecpar);
    averr_process(check_err);
    if (avcodec_open2(codec_ctx, codec, nullptr) != 0) {
        throw PlayAudioErr("Could not open codec");
    }

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
    check_err = swr_init(swr_context);
    averr_process(check_err);

    // создание буферов
    AVPacket * pkt; // structure stores compressed data
    if ((pkt = av_packet_alloc()) == nullptr) {
        throw PlayAudioErr("Could not allocate audio packet");
    }
    AVFrame* frame; // structure describes decoded (raw) audio data
    if ((frame = av_frame_alloc()) == nullptr) {
        throw PlayAudioErr("Could not allocate audio frame");
    }


    PlayPcm(); // запускаем сразу

    // цикл чтения файла
    while ((check_err = av_read_frame(fmt_ctx, pkt)) == 0) {
        {
            // Остановка декодировая на момент паузы, чтобы не тратить ресурсы устройства
            std::unique_lock<std::mutex> lock(pause_mtx);
            pause_cv.wait(lock, []{return isplaying.load();});
        }
        if (pkt->stream_index == stream_index) {
            check_err = avcodec_send_packet(codec_ctx, pkt);
            if (check_err < 0 && check_err != AVERROR_EOF && check_err != AVERROR(EAGAIN)) {
                averr_process(check_err);
            }
            while (!(check_err = avcodec_receive_frame(codec_ctx, frame))) {
                uint8_t* out_data;
                int out_linesize;
                if (av_samples_alloc(&out_data, &out_linesize, 2, frame->nb_samples, AV_SAMPLE_FMT_S16, 0) < 0) {
                    throw PlayAudioErr("Could not allocate audio data(samples)");
                }
                int samples = swr_convert(swr_context, &out_data, frame->nb_samples, (const uint8_t**)frame->data, frame->nb_samples);
                if (samples < 0) {
                    throw PlayAudioErr("Could not convert audio data(samples)");
                }
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
            if (check_err < 0 && check_err != AVERROR_EOF && check_err != AVERROR(EAGAIN)) {
                averr_process(check_err);
            }
        }
        av_packet_unref(pkt);
    }
    if (check_err != AVERROR_EOF) {
        averr_process(check_err);
    }
    // free memory
    av_frame_free(&frame);
    av_packet_free(&pkt);
    swr_free(&swr_context);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);
}

double GetDurationWithFFprobe(const char* filename) {
    AVFormatContext* fmt_ctx = nullptr;
    AVDictionary* options = nullptr;

    av_dict_set(&options, "accurate_seek", "1", 0); // установка опций для более точного определения длительности

    if (avformat_open_input(&fmt_ctx, filename, nullptr, &options) < 0) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        av_dict_free(&options);
        return -1;
    }

    av_dict_free(&options);

    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        std::cerr << "Cannot find stream info" << std::endl;
        avformat_close_input(&fmt_ctx);
        return -1;
    }

    double duration = -1;

    // Способ 1: Длительность из контейнера
    if (fmt_ctx->duration != AV_NOPTS_VALUE) {
        duration = (double)fmt_ctx->duration / AV_TIME_BASE;
    }
    // Способ 2: Длительность из аудио потока
    int audio_stream_index = -1;
    for (unsigned int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            AVStream* stream = fmt_ctx->streams[i];

            if (stream->duration != AV_NOPTS_VALUE) {
                double stream_duration = stream->duration * av_q2d(stream->time_base);

                // Выбираем длительность потока, если она доступна
                if (stream_duration > 0) {
                    duration = stream_duration;
                }
            }
            break;
        }
    }

    avformat_close_input(&fmt_ctx);
    return duration;
}

void ResumePlay() {
    if (!isplaying) {
        isplaying = true;
        pause_cv.notify_all();
        AudioQueueStart(queue, nullptr);
    }
}

void Pause() {
    if (isplaying) {
        isplaying = false;
        AudioQueuePause(queue);
    }
}

void ResetPlay() {
    // Очищение всех данных, чтобы новый трек не наложился на старый и тд.
    // Вызывать перед запуском нового трека
    if (queue) {
        AudioQueueStop(queue, true);
        AudioQueueDispose(queue, true);
        queue = nullptr;
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::queue<std::vector<uint8_t>> empty;
        std::swap(que, empty);
    }
}

void averr_process(int err) {
    if (err < 0) {
        char errbuf[256];
        av_strerror(err, errbuf, sizeof(errbuf));
        throw PlayAudioErr(errbuf);
    }
}

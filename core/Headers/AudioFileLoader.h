#ifndef AUDIOFILELOADER_H
#define AUDIOFILELOADER_H
#include "deque"
#include "filesystem"

std::deque<std::filesystem::path> GetAudioFiles(const char* ptd);
#endif //AUDIOFILELOADER_H
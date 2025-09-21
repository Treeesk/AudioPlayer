#include "PlayAudio.h"
int main() {
    const char* path = "/Applications/AudioPlayer/testingmp3mus/ANNA_ASTI_-_Po_baram_74376135.mp3";
    try {
        PlayAudio(path);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
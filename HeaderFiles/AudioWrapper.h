#ifndef AUDIOWRAPPER_H
#define AUDIOWRAPPER_H
#include <QWidget>
#include "PlayAudio.h"

class AudioWorker: public QObject {
    Q_OBJECT
public:
    AudioWorker(): player(new Player) {}
    ~AudioWorker() {delete player;}
public slots:
    void seekAudio(int value);
    void playTrack(const char* path);
    void ResumeTrack();
    void pauseTrack();
private:
    Player* player;
};

#endif // AUDIOWRAPPER_H

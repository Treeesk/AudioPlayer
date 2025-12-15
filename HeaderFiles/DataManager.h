#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QObject>
#include "track.h"
#include "PlayAudio.h"
#include "AudioWrapper.h"
#include "QThread"
double GetDuration(const char* path);

class MusicDataManager: public QObject {
    Q_OBJECT
public:
    MusicDataManager(QObject* parent = nullptr);
    ~MusicDataManager();
    void loadfromdata(const char* path_to_dir);
    const track& currenttrack();
    const QVector<track>& alltracks();

public slots:
    void play();
    void pause();
    void next();
    void prev();
    void seekingAudio(int value);
    void setVolume(int value);
    void PanelChangeTrack(int index);
signals:
    void currenttrackchange(const track& trk);
    void playRequested(const char* path);
    void resumeRequested();
    void pauseRequested();
    void seekRequested(int value, const char* path);
    void NewVolRequested(int value);
private:
    QVector<track> tracks;
    int _currenttrackind = -1;
    bool _isplaying = false;
    bool launchtrack = false;
    AudioWorker* worker;
    QThread audioThread;
};

#endif // DATAMANAGER_H

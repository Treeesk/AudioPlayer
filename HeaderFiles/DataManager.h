#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QObject>
#include "track.h"
class MusicDataManager: public QObject {
    Q_OBJECT
public:
    MusicDataManager(QObject* parent = nullptr): QObject(parent) {}
    void loadfromdata(const char* path_to_dir);
    const track& currenttrack();
    const QVector<track>& alltracks();

public slots:
    void play();
    void pause();
    void next();
    void prev();
signals:
//curtrackchange, обработчик ошибок

private:
    QVector<track> tracks;
    int _currenttrackind = -1;
    bool _isplaying = false;
};

#endif // DATAMANAGER_H

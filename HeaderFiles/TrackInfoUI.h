#ifndef TRACKINFOUI_H
#define TRACKINFOUI_H
#include <QWidget>
#include "track.h"
#include <QTimer>
class TrackTime: public QWidget {
    Q_OBJECT
private:
    int currenttime;
    int totaltime;
    QTimer* timer;
    int _width;
    int _height;
    bool launch = false;
public:
    TrackTime(int duration, int width, int height, QWidget *parent = nullptr);
    void settime(int duration);
protected:
    void paintEvent(QPaintEvent* event);
public slots:
    void changetime();
    void start();
    void pause();
};

class TrackInfoWidget: public QWidget{
    Q_OBJECT
private:
    QPixmap pixmapcover;
    QString artist;
    QString title;
    int sizecover = 200;
public:
    TrackTime* time;
    TrackInfoWidget(const track& trk, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event);
public slots:
    void setTrack(const track& trk);
};

#endif // TRACKINFOUI_H

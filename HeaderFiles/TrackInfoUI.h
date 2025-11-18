#ifndef TRACKINFOUI_H
#define TRACKINFOUI_H
#include <QWidget>
#include "track.h"
#include <QTimer>
#include <QSlider>
class TrackTime: public QWidget {
    Q_OBJECT
private:
    int currenttime;
    int totaltime;
    QTimer* timer;
    QSlider* slider;
    bool launch = false;
    bool flag_for_slider_geo = false;
public:
    TrackTime(const int& duration, const int& width, const int& height, QWidget *parent = nullptr);
    void settime(const int& duration);
protected:
    void paintEvent(QPaintEvent* event);
public slots:
    void changetime();
    void start();
    void pause();
signals:
    void endtrack();
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

#ifndef TRACKINFOUI_H
#define TRACKINFOUI_H
#include <QWidget>
#include "track.h"
#include <QTimer>
#include <QSlider>
#include "TrackInfoAbstract.h"
class TrackTime: public QWidget {
    Q_OBJECT
private:
    int currenttime;
    int totaltime;
    QTimer* timer;
    QSlider* slider;
    bool launch = false;
    bool flag_for_slider_geo = false;
    bool seeking = false;
public:
    TrackTime(const int& duration, const int& width, const int& height, QWidget *parent = nullptr);
    void settime(const int& duration);
protected:
    void paintEvent(QPaintEvent* event);
public slots:
    void changetime();
    void start();
    void pause();
    void setnewTime(int value);
signals:
    void endtrack();
    void setTimeTrack(int value);
    void SeekChangeIcon();
};

class TrackInfoWidget: public TrackInfoBase {
    Q_OBJECT
private:
    int sizecover = 200;
public:
    TrackTime* time;
    TrackInfoWidget(const int& _width, const int& _height, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event) override;
public slots:
    void setTrack(const track& trk);
};
#endif // TRACKINFOUI_H

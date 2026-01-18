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
    bool seeking = false;
public:
    TrackTime(QWidget *parent = nullptr);
    void settime(const int& duration);
    void initTime(const int& duration);
protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
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
    QRect coverRect;
    QRect titleRect;
    QRect artistRect;
    const int TIMER_HEIGHT = 15;
    const int SPACING = 5;
    const int MIN_TEXT = 40;
    int fontSizeforTitle = 16;
    int fontSizeforArtist = 14;
public:
    TrackTime* time;
    TrackInfoWidget(QWidget *parent = nullptr);
    virtual QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
public slots:
    void setTrack(const track& trk);
    void initTrack(const track& trk);
};
#endif // TRACKINFOUI_H

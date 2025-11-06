#ifndef TRACKINFOUI_H
#define TRACKINFOUI_H
#include <QWidget>
#include "track.h"
class TrackInfoWidget: public QWidget{
    Q_OBJECT
private:
    QPixmap pixmapcover;
    QString artist;
    QString title;
    int sizecover = 200;
public:
    TrackInfoWidget(const track& trk, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event);
public slots:
    void setTrack(const track& trk);
};
#endif // TRACKINFOUI_H

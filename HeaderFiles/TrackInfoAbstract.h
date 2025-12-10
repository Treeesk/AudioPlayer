#ifndef TRACKINFOABSTRACT_H
#define TRACKINFOABSTRACT_H
#include <QWidget>
#include "track.h"
class TrackInfoBase: public QWidget {
    Q_OBJECT
private:
    QPixmap pixmapcover;
    QString artist;
    QString title;
    int durationTrack;
public:
    TrackInfoBase(const track& trk, QWidget* parent = nullptr);
    virtual ~TrackInfoBase() = default;
    void changeTrack(const track& trk);
    QPixmap getCover();
    QString getArtist();
    QString getTitle();
    int getDuration();
protected:
    virtual void paintEvent(QPaintEvent* event) = 0;
};

#endif // TRACKINFOABSTRACT_H

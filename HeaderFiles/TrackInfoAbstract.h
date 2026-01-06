#ifndef TRACKINFOABSTRACT_H
#define TRACKINFOABSTRACT_H
#include <QWidget>
#include "track.h"
#include <optional>
class TrackInfoBase: public QWidget {
    Q_OBJECT
private:
    std::optional<track> _track;
    QPixmap pixmapcover;
    QString artist;
    QString title;
    int durationTrack;
public:
    TrackInfoBase(QWidget* parent = nullptr);
    virtual ~TrackInfoBase() = default;
    void changeTrack(const track& trk);
    QPixmap getCover();
    QString getArtist();
    QString getTitle();
    int getDuration();
    bool hasTrack();
protected:
    virtual void paintEvent(QPaintEvent* event) = 0;
};

#endif // TRACKINFOABSTRACT_H

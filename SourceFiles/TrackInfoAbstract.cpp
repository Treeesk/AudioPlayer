#include "TrackInfoAbstract.h"

TrackInfoBase::TrackInfoBase(const track& trk, QWidget* parent): QWidget(parent) {
    changeTrack(trk);
}

void TrackInfoBase::changeTrack(const track& trk) {
    pixmapcover = QPixmap::fromImage(trk.cover);
    artist = trk._artist;
    title = trk._title;
    durationTrack = trk._duration;
}

QPixmap TrackInfoBase::getCover() {
    return pixmapcover;
}

QString TrackInfoBase::getArtist() {
    return artist;
}

QString TrackInfoBase::getTitle() {
    return title;
}

int TrackInfoBase::getDuration() {
    return durationTrack;
}

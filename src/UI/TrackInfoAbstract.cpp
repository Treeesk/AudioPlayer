#include "UI/TrackInfoAbstract.h"

TrackInfoBase::TrackInfoBase(QWidget* parent): QWidget(parent) {
}

void TrackInfoBase::changeTrack(const track& trk) {
    _track = trk;
    pixmapcover = QPixmap::fromImage(trk.cover);
    artist = trk._artist;
    title = trk._title;
    durationTrack = trk._duration;
}

QPixmap TrackInfoBase::getCover() {
    return hasTrack() ? pixmapcover : QPixmap();
}

QString TrackInfoBase::getArtist() {
    return hasTrack() ? artist : QString();
}

QString TrackInfoBase::getTitle() {
    return hasTrack() ? title : QString();
}

int TrackInfoBase::getDuration() {
    return hasTrack() ? durationTrack : 0;
}

bool TrackInfoBase::hasTrack() {
    return _track.has_value();
}

#include "TrackInfoUI.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>
TrackInfoWidget::TrackInfoWidget(const track& trk, QWidget *parent): QWidget(parent) {
    setFixedSize(sizecover, sizecover + 60);
    pixmapcover = QPixmap::fromImage(trk.cover);
    artist = trk._artist;
    title = trk._title;
}

void TrackInfoWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    // if (!pixmapcover.isNull()) {
        QRect coverRect((width() - sizecover) / 2.0, 0, sizecover, sizecover); // размер 200x200, начало в 0 0
    // }
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(coverRect, pixmapcover);
    QRect titleRect(0, sizecover + 10, sizecover, 20);
    painter.drawText(titleRect, Qt::AlignCenter | Qt::TextWordWrap, title);
    QRect artistRect(0, sizecover + 40, sizecover, 20);
    painter.drawText(artistRect, Qt::AlignCenter | Qt::TextWordWrap, artist);
}

void TrackInfoWidget::setTrack(const track& trk) {
    pixmapcover = QPixmap::fromImage(trk.cover);
    artist = trk._artist;
    title = trk._title;
    // возможно update прописать
}

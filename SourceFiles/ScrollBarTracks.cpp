#include "ScrollBarTracks.h"
#include <QPainter>

TrackInfoRepresentation::TrackInfoRepresentation(const track& trk, QWidget* parent): TrackInfoBase(trk, parent) {
}

void TrackInfoRepresentation::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QRect coverRect(0, 0, width() / 2, height());
    painter.setPen(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(coverRect, getCover());

    QFont font("Times New Roman", 12, QFont::DemiBold);
    painter.setFont(font);
    QRect titleRect(width() / 2 + 5, 0, width() / 2 - 5, height() / 2);
    painter.drawText(titleRect, Qt::AlignLeft | Qt::TextWordWrap | Qt::AlignBottom, getTitle());

    font.setPointSize(10);
    font.setWeight(QFont::Normal);
    painter.setFont(font);
    QRect artistRect(width() / 2 + 5, height() / 2 + 2, width() / 2 - 5, height() / 2 - 2);
    painter.drawText(artistRect, Qt::AlignLeft | Qt::TextWordWrap | Qt::AlignTop, getArtist());
}

TrackInfoScroll::TrackInfoScroll(const QVector<track>& tracks, QWidget* parent): QWidget(parent) {
    layout = new QVBoxLayout(this);
    layout->setSpacing(5);
    layout->setAlignment(Qt::AlignTop);
    loadTracks(tracks);
    layout->addStretch();
}

void TrackInfoScroll::loadTracks(const QVector<track>& tracks) {
    for (const track& trk: tracks) {
        TrackInfoRepresentation* item = new TrackInfoRepresentation(trk, this);
        layout->addWidget(item);
        track_widgets.push_back(item);
    }
}

void TrackInfoScroll::resizeEvent(QResizeEvent* event) {
    int min_width = 100;
    int max_width = 225;
    int set_width = width();
    if (set_width < min_width) {
        set_width = min_width;
    }
    else if (set_width > max_width) {
        set_width = max_width;
    }
    QWidget::resizeEvent(event);
    for (auto* widget: track_widgets) {
        widget->setFixedSize(set_width, set_width / 2);
    }
}

#include "ScrollBarTracks.h"
#include <QPainter>
#include <algorithm>
#include <QMouseEvent>

void TrackInfoRepresentation::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit setTrackFromPanel(index_of_track);
    }
    TrackInfoBase::mouseReleaseEvent(event);
}

void TrackInfoRepresentation::setSelected(bool flag) {
    selectedTrack = flag;
    update();
}

TrackInfoRepresentation::TrackInfoRepresentation(const track& trk, int index, bool flag, QWidget* parent)
    : TrackInfoBase(parent) {
    changeTrack(trk);
    index_of_track = index;
    selectedTrack = flag;
}

void TrackInfoRepresentation::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);

    if (selectedTrack) {
        painter.fillRect(rect(), QColor(192, 192, 192));
    }

    QRect coverRect(0, 0, width() / 2, height());
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

TrackInfoScroll::TrackInfoScroll(QWidget* parent)
    : QWidget(parent) {

    mainLayuot = new QVBoxLayout(this);
    mainLayuot->setContentsMargins(0, 0, 0, 0);
    mainLayuot->setSpacing(0);

    containerWidget = new QWidget;
    containerWidget->setStyleSheet("background: transparent;");

    scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            border: none;
            background: transparent;
        }
    )");

    layout = new QVBoxLayout(containerWidget);
    layout->setSpacing(5);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(5, 5, 0, 5);

    // привязываем containerWidget к scrollArea
    scrollArea->setWidget(containerWidget);
    mainLayuot->addWidget(scrollArea); // нужно сказать layuout, что он отвечает за геометрию scrollarea(ну или виджета)
}

void TrackInfoScroll::loadTracks(const QVector<track>& tracks) {
    clear_tracks();
    TrackInfoRepresentation* item = new TrackInfoRepresentation(tracks[0], 0, true, containerWidget);
    current = item;
    connect(item, &TrackInfoRepresentation::setTrackFromPanel, this, &TrackInfoScroll::clicked);
    layout->addWidget(item);
    track_widgets.push_back(item);
    for (int i = 1; i < tracks.size(); ++i) {
        TrackInfoRepresentation* item = new TrackInfoRepresentation(tracks[i], i, false, containerWidget);
        connect(item, &TrackInfoRepresentation::setTrackFromPanel, this, &TrackInfoScroll::clicked);
        layout->addWidget(item);
        track_widgets.push_back(item);
    }
    update();
}

void TrackInfoScroll::clear_tracks() {
    if (!track_widgets.empty()) {
        for (auto& widget: track_widgets) {
            layout->removeWidget(widget);
            widget->deleteLater();
        }
        track_widgets.clear();
        current = nullptr;
    }
}

void TrackInfoScroll::paintEvent(QPaintEvent* event) {
    int min_width = 100;
    int max_width = 225;
    int set_width = width();
    set_width = std::clamp(set_width, min_width, max_width);

    const int spacing = layout->spacing();
    QMargins m = layout->contentsMargins();

    int totalHeight = m.top() + m.bottom();
    if (!track_widgets.empty()) {
        bool first = true;
        for (auto* widget : track_widgets) {
            widget->setFixedSize(set_width, set_width / 2);
            if (!first)
                totalHeight += spacing;
            totalHeight += widget->height();
            first = false;
        }
    }
    containerWidget->setFixedSize(set_width, totalHeight);
}

void TrackInfoScroll::clicked(int index) {
    if (current != track_widgets[index]) {
        current->setSelected(false);
        current = track_widgets[index];
        current->setSelected(true);
    }
    emit SetNewTrackPanel(index);
}

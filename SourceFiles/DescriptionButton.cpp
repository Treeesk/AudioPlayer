#include "DescriptionButton.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

PlayerControlsWidget::PlayerControlsWidget(QWidget* parent): QWidget(parent) {
    QIcon playIcon(":/Resources/Icons/Play.png");
    QIcon stopIcon(":/Resources/Icons/Stop.png");
    QIcon nexttrackIcon(":/Resources/Icons/Next.png");
    QIcon prevtrackIcon(":/Resources/Icons/Prev.png");
    PlayStopTrack = new RoundPushButton(this);
    NextTrack = new RoundPushButton(this);
    PrevTrack = new RoundPushButton(this);
    NextTrack->setIcon(nexttrackIcon);
    NextTrack->setIconSize(QSize(35, 35)); // тест
    NextTrack->setFixedSize(35, 35); // тест
    NextTrack->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    PrevTrack->setIconSize(QSize(35, 35)); // тест
    PrevTrack->setFixedSize(35, 35); // тест
    PrevTrack->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    PrevTrack->setIcon(prevtrackIcon);

    PlayStopTrack->setCheckable(true);
    PlayStopTrack->setIcon(playIcon);
    PlayStopTrack->setIconSize(QSize(35, 35)); // тест
    PlayStopTrack->setFixedSize(35, 35); // тест
    PlayStopTrack->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(PlayStopTrack, &QPushButton::toggled, this, [this, playIcon, stopIcon](bool checked) {
        if (!checked) {
            this->PlayStopTrack->setIcon(playIcon);
        }
        else {
            this->PlayStopTrack->setIcon(stopIcon);
        }
    });
    UpdateButtonsGeo();
    PrevTrack->setToolTip("Предыдущий трек");
    NextTrack->setToolTip("Следующий трек");
    PlayStopTrack->setToolTip("Пауза/Воспроизведение");
}

void PlayerControlsWidget::UpdateButtonsGeo() {
    int spacing = 10;
    _widthPCW = PlayStopTrack->size().width() + PrevTrack->size().width() + NextTrack->size().width() + spacing * 2;
    int startX = (width() - _widthPCW) / 2;
    int startY = (height() - PlayStopTrack->size().height()) / 2;
    PrevTrack->setGeometry(startX, startY, PrevTrack->size().width(), PrevTrack->size().height());
    PlayStopTrack->setGeometry(startX + PrevTrack->size().width() + spacing, startY, PlayStopTrack->size().width(), PlayStopTrack->size().height());
    NextTrack->setGeometry(startX + PrevTrack->size().width() + PlayStopTrack->size().width() + 2 * spacing, startY, NextTrack->size().width(), NextTrack->size().height());
}

void PlayerControlsWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    UpdateButtonsGeo();
}

int PlayerControlsWidget::WidthPCW() {
    return _widthPCW;
}

bool RoundPushButton::hitButton(const QPoint& pos) const {
    QSize iconsize = this->iconSize();
    double radius = iconsize.width() / 2.0;
    double dist_from_center_x = pos.x() - width() / 2.0;
    double dist_from_center_y = pos.y() - height() / 2.0;
    return (dist_from_center_x * dist_from_center_x + dist_from_center_y * dist_from_center_y) <= (radius * radius);
}

void RoundPushButton::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // сглаживание краев
    QPainterPath path;
    QPixmap img = this->icon().pixmap(this->iconSize());
    QPointF topLeft((width() - this->iconSize().width()) / 2.0, (height() - this->iconSize().height()) / 2.0);
    path.addEllipse(QRectF(topLeft, this->iconSize()));
    painter.setClipPath(path);
    painter.drawPixmap(topLeft, img);
    painter.setPen(Qt::NoPen);
    painter.drawPath(path);
}

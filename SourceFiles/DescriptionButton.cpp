#include "DescriptionButton.h"
#include <QPainter>
#include <QPainterPath>

PlayerControlsWidget::PlayerControlsWidget(QWidget* parent): QWidget(parent) {
    playIcon = QIcon(":/Resources/Icons/Play.png");
    stopIcon = QIcon(":/Resources/Icons/Stop.png");
    nexttrackIcon = QIcon(":/Resources/Icons/Next.png");
    prevtrackIcon = QIcon(":/Resources/Icons/Prev.png");
    PlayStopTrack = new RoundPushButton(this);
    NextTrack = new RoundPushButton(this);
    PrevTrack = new RoundPushButton(this);
    NextTrack->setIcon(nexttrackIcon);
    NextTrack->setIconSize(QSize(30, 30)); // тест
    NextTrack->setFixedSize(30, 30); // тест
    NextTrack->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    NextTrack->setCheckable(true);
    connect(NextTrack, &QPushButton::toggled, this, &PlayerControlsWidget::NextTrackclick);

    PrevTrack->setIconSize(QSize(30, 30)); // тест
    PrevTrack->setFixedSize(30, 30); // тест
    PrevTrack->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    PrevTrack->setIcon(prevtrackIcon);
    PrevTrack->setCheckable(true);
    connect(PrevTrack, &QPushButton::toggled, this, &PlayerControlsWidget::PrevTrackclick);

    PlayStopTrack->setCheckable(true);
    PlayStopTrack->setIcon(playIcon);
    PlayStopTrack->setIconSize(QSize(30, 30)); // тест
    PlayStopTrack->setFixedSize(30, 30); // тест
    PlayStopTrack->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(PlayStopTrack, &QPushButton::toggled, this, &PlayerControlsWidget::PlayStopclick);
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

void PlayerControlsWidget::PlayStopclick(bool isPlaying) {
    if (isPlaying) {
        this->PlayStopTrack->setIcon(stopIcon);
        emit onPlayclicked();
    }
    else {
        this->PlayStopTrack->setIcon(playIcon);
        emit onPauseclicked();
    }
}

void PlayerControlsWidget::NextTrackclick() {
    ChangeIcon();
    emit onNextclicked();
}

void PlayerControlsWidget::PrevTrackclick(){
    ChangeIcon();
    emit onPrevclicked();
}

void PlayerControlsWidget::ChangeIcon() {
    if (!PlayStopTrack->isChecked()) { // проверяем состояние кнопки
        PlayStopTrack->blockSignals(true); // выключили сигналы от PlayStopTrack
        PlayStopTrack->setChecked(true);
        PlayStopTrack->setIcon(stopIcon); // показывает кнопку остановки(трек проигрывается)
        PlayStopTrack->blockSignals(false);
    }
}

void PlayerControlsWidget::ResetIcon() {
    if (PlayStopTrack->isChecked()) { // проверяем состояние кнопки
        PlayStopTrack->blockSignals(true); // выключили сигналы от PlayStopTrack
        PlayStopTrack->setChecked(false);
        PlayStopTrack->setIcon(playIcon); // показывает кнопку старта(трек на паузе)
        PlayStopTrack->blockSignals(false);
    }
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


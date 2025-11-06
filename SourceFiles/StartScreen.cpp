#include "StartScreen.h"

StartScreenWidget::StartScreenWidget(MusicDataManager* mdm, QWidget* parent): QWidget(parent) {
    _mdm = mdm;
    pcw = new PlayerControlsWidget(this);
    trif = new TrackInfoWidget(_mdm->currenttrack(), this);
    create_connections();
    UpdateScreenPos();
}

void StartScreenWidget::UpdateScreenPos() {
    int centerX = width() / 2;
    int verticalSpacing = 10;
    int pcwCenterX = centerX + (trif->width() - pcw->WidthPCW()) / 2;
    int startY = height() / 4;
    int pcwY = startY + 260 + verticalSpacing;

    trif->setGeometry(centerX, startY, 200, 260);
    pcw->setGeometry(pcwCenterX, pcwY, pcw->WidthPCW(), 70);
}

void StartScreenWidget::resizeEvent(QResizeEvent* event) {
    UpdateScreenPos();
    QWidget::resizeEvent(event);
}

void StartScreenWidget::create_connections() {
    connect(pcw, &PlayerControlsWidget::onPlayclicked, _mdm, &MusicDataManager::play);
    connect(pcw, &PlayerControlsWidget::onPauseclicked, _mdm, &MusicDataManager::pause);
    connect(pcw, &PlayerControlsWidget::onNextclicked, _mdm, &MusicDataManager::next);
    connect(pcw, &PlayerControlsWidget::onPrevclicked, _mdm, &MusicDataManager::prev);
}

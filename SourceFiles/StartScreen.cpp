#include "StartScreen.h"

StartScreenWidget::StartScreenWidget(MusicDataManager* mdm, QWidget* parent): QWidget(parent) {
    _mdm = mdm;
    pcw = new PlayerControlsWidget(this);
    create_connections();
    UpdateScreenPos();
}

void StartScreenWidget::UpdateScreenPos() {
    int centerX = width() / 2;
    pcw->setGeometry(centerX, height() * 3 / 4, pcw->WidthPCW(), 70);
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

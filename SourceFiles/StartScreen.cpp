#include "StartScreen.h"
#include <QDebug>

StartScreenWidget::StartScreenWidget(MusicDataManager* mdm, QWidget* parent): QWidget(parent) {
    _mdm = mdm;
    pcw = new PlayerControlsWidget(this);

    trif_height = 270;
    trif_width = 200;
    trif = new TrackInfoWidget(_mdm->currenttrack(), trif_width, trif_height, this);

    vol_width = 200;
    vol_height = 20;
    vol = new volumeSlider(vol_width, vol_height, this);

    scrollPanel = new TrackInfoScroll(_mdm->alltracks(), this);

    create_connections();
    UpdateScreenPos();
}

void StartScreenWidget::UpdateScreenPos() {
    int centerX = width() / 2;
    int verticalSpacing = 1;
    int startY = height() / 4;

    int pcwCenterX = centerX + (trif_width - pcw->WidthPCW()) / 2;
    int pcwY = startY + trif_height + verticalSpacing;
    int pcwHeight  = 40;

    int volY = pcwY + pcwHeight + verticalSpacing;

    trif->setGeometry(centerX, startY, trif_width, trif_height);
    pcw->setGeometry(pcwCenterX, pcwY, pcw->WidthPCW(), pcwHeight);
    vol->setGeometry(centerX, volY, vol_width, vol_height);

    scrollPanel->setGeometry(0, 0, width() / 4, height());
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
    connect(_mdm, &MusicDataManager::currenttrackchange, trif, &TrackInfoWidget::setTrack);
    connect(pcw, &PlayerControlsWidget::onPlayclicked, trif->time, &TrackTime::start);
    connect(pcw, &PlayerControlsWidget::onPauseclicked, trif->time, &TrackTime::pause);
    connect(trif->time, &TrackTime::endtrack, _mdm, &MusicDataManager::next);
    connect(trif->time, &TrackTime::setTimeTrack, _mdm, &MusicDataManager::seekingAudio);
    connect(trif->time, &TrackTime::SeekChangeIcon, pcw, &PlayerControlsWidget::ChangeIcon);
    connect(vol, &volumeSlider::newVolume, _mdm, &MusicDataManager::setVolume);
    connect(scrollPanel, &TrackInfoScroll::SetNewTrackPanel, _mdm, &MusicDataManager::PanelChangeTrack);
}


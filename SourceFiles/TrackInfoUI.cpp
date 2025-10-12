#include "TrackInfoUI.h"
#include <QHBoxLayout>


PlayerControlsWidget::PlayerControlsWidget(QWidget* parent): QWidget(parent) {
    QIcon playIcon(":/Resources/Icons/Play.png");
    QIcon stopIcon(":/Resources/Icons/Stop.png");
    PlayStopTrack = new QPushButton(this);
    PlayStopTrack->setCheckable(true);
    PlayStopTrack->setIcon(playIcon);
    PlayStopTrack->setIconSize(QSize(35, 35));
    PlayStopTrack->setFixedSize(50, 50);
    connect(PlayStopTrack, &QPushButton::toggled, this, [this, playIcon, stopIcon](bool checked) {
        if (!checked) {
            this->PlayStopTrack->setIcon(playIcon);
        }
        else {
            this->PlayStopTrack->setIcon(stopIcon);
        }
    });
    QHBoxLayout* layout = new QHBoxLayout(this); // лэйаут подключаем к виджету, и добавляем кнопки виджета к лэйауту
    layout->addWidget(PlayStopTrack);
}

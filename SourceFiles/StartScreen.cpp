#include "StartScreen.h"
#include <QDebug>

StartScreenWidget::StartScreenWidget(MusicDataManager* mdm, QWidget* parent): QWidget(parent) {
    _mdm = mdm;
    pcw = new PlayerControlsWidget(this);
    pcw->hide();

    trif_height = 270;
    trif_width = 200;
    trif = new TrackInfoWidget(this);
    trif->hide();

    vol_width = 200;
    vol_height = 20;
    vol = new volumeSlider(vol_width, vol_height, this);
    vol->hide();

    scrollPanel = new TrackInfoScroll(this);
    scrollPanel->hide();

    Directory = new OpenDirectoryButton(this);

    create_connections();
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
    Directory->setGeometry(width() - 80, 10, 70, 35);
}

void StartScreenWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    UpdateScreenPos();
}

void StartScreenWidget::create_connections() {
    connect(pcw, &PlayerControlsWidget::onPlayclicked, _mdm, &MusicDataManager::play); // Запуск/возобновление проигрывания трека
    connect(pcw, &PlayerControlsWidget::onPauseclicked, _mdm, &MusicDataManager::pause); // Пауза
    connect(pcw, &PlayerControlsWidget::onNextclicked, _mdm, &MusicDataManager::next); // Переключение на следующий трек
    connect(pcw, &PlayerControlsWidget::onPrevclicked, _mdm, &MusicDataManager::prev); // Переключение на предыдущий трек
    connect(pcw, &PlayerControlsWidget::onPlayclicked, trif->time, &TrackTime::start); // По нажатию на кнопку Запуск/возобновление, начинает идти таймер
    connect(pcw, &PlayerControlsWidget::onPauseclicked, trif->time, &TrackTime::pause); //  По нажатию на кнопку Пауза, начинает идти таймер

    connect(_mdm, &MusicDataManager::currenttrackchange, trif, &TrackInfoWidget::setTrack); // При смене трека, меняется внешнее представление на актуальный трек
    connect(_mdm, &MusicDataManager::setTrackfromDir, trif, &TrackInfoWidget::initTrack); // установка трека при открытии папки с музыкой
    connect(_mdm, &MusicDataManager::setTracksfromDir, scrollPanel, &TrackInfoScroll::loadTracks); // установка треков после открытии папки с музыкой
    connect(_mdm, &MusicDataManager::tracksLoaded, this, &StartScreenWidget::showWidgets); // При инициализации треков из папки виджеты на экране должны отобразиться
    connect(_mdm, &MusicDataManager::currentTrackIndexChanged, scrollPanel, &TrackInfoScroll::clicked); // передача индекса акутального трека
    connect(_mdm, &MusicDataManager::resetPCW, pcw, &PlayerControlsWidget::ResetIcon); // После открытия папки сброс кнопок

    connect(trif->time, &TrackTime::endtrack, _mdm, &MusicDataManager::next); // Если трек заканчивается(проигран) переключается на следующий
    connect(trif->time, &TrackTime::setTimeTrack, _mdm, &MusicDataManager::seekingAudio); // При перемотке трека в UI, перематывается трек внутри движка
    connect(trif->time, &TrackTime::SeekChangeIcon, pcw, &PlayerControlsWidget::ChangeIcon); // Смена иконки, если пользователь перемотал и у него была иконка паузы(смена на иконку проигрывания трека)

    connect(vol, &volumeSlider::newVolume, _mdm, &MusicDataManager::setVolume); // Изменения громкости музыки

    connect(scrollPanel, &TrackInfoScroll::SetNewTrackPanel, _mdm, &MusicDataManager::PanelChangeTrack); // При переключении трека на панели слева, перерисовка основного вида трека
    connect(scrollPanel, &TrackInfoScroll::SetNewTrackPanel, pcw, &PlayerControlsWidget::ChangeIcon); // смена иконки с паузы на проигрывание(если она стояла)

    connect(Directory, &OpenDirectoryButton::dirFound, _mdm, &MusicDataManager::loadfromdata); // открытие папки по кнопке и вызов загрузки треков
}

void StartScreenWidget::showWidgets() {
    pcw->show();
    trif->show();
    vol->show();
    scrollPanel->show();
}

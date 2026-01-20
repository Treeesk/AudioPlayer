#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#include "DescriptionButton.h"
#include "DataManager.h"
#include "TrackInfoUI.h"
#include "VolumeSlider.h"
#include "ScrollBarTracks.h"
#include "OpenDirectory.h"

class StartScreenWidget: public QWidget {
    Q_OBJECT
public:
    StartScreenWidget(MusicDataManager* mdm, QWidget* parent = nullptr);
private:
    PlayerControlsWidget* pcw;
    MusicDataManager* _mdm;
    TrackInfoWidget* trif;
    volumeSlider* vol;
    TrackInfoScroll* scrollPanel;
    OpenDirectoryButton* Directory; // кнопка открытия файлового окна для выбора папки откуда будет проигрываться музыка
    void UpdateScreenPos();
    void create_connections();
protected:
    void resizeEvent(QResizeEvent* event);
public slots:
    void showWidgets();
};

#endif // STARTSCREEN_H

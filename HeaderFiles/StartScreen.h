#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#include "DescriptionButton.h"
#include "DataManager.h"
#include "TrackInfoUI.h"

class StartScreenWidget: public QWidget {
    Q_OBJECT
public:
    StartScreenWidget(MusicDataManager* mdm, QWidget* parent = nullptr);
private:
    PlayerControlsWidget* pcw;
    MusicDataManager* _mdm;
    TrackInfoWidget* trif;
    void UpdateScreenPos();
    void create_connections();
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // STARTSCREEN_H

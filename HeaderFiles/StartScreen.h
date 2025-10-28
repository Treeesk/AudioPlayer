#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#include "DescriptionButton.h"
#include "DataManager.h"

class StartScreenWidget: public QWidget {
    Q_OBJECT
public:
    StartScreenWidget(MusicDataManager* mdm, QWidget* parent = nullptr);
private:
    PlayerControlsWidget* pcw;
    MusicDataManager* _mdm;
    void UpdateScreenPos();
    void create_connections();
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // STARTSCREEN_H

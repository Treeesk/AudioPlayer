#ifndef TRACKINFOUI_H
#define TRACKINFOUI_H
#include <QPushButton>

class PlayerControlsWidget: public QWidget{
private:
    QPushButton *PlayStopTrack;
    QPushButton *PrevTrack;
    QPushButton *NextTrack;
public:
    PlayerControlsWidget(QWidget *parent = nullptr);
};
#endif // TRACKINFOUI_H

#ifndef TRACKINFOUI_H
#define TRACKINFOUI_H
#include <QPushButton>

class RoundPushButton: public QPushButton {
    Q_OBJECT
public:
    RoundPushButton(QWidget *parent = nullptr): QPushButton(parent) {}
protected:
    bool hitButton(const QPoint& pos) const;
    void paintEvent(QPaintEvent* event);
};

class PlayerControlsWidget: public QWidget{
    Q_OBJECT
private:
    RoundPushButton *PlayStopTrack;
    RoundPushButton *PrevTrack;
    RoundPushButton *NextTrack;
public:
    PlayerControlsWidget(QWidget *parent = nullptr);
};
#endif // TRACKINFOUI_H

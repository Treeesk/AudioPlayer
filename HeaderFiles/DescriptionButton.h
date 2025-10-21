#ifndef DESCRIPTIONBUTTON_H
#define DESCRIPTIONBUTTON_H
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
    int _widthPCW;
    void UpdateButtonsGeo();
public:
    PlayerControlsWidget(QWidget *parent = nullptr);
    int WidthPCW();
protected:
    void resizeEvent(QResizeEvent* event);
};
#endif // DESCRIPTIONBUTTON_H

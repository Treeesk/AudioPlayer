#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#include "DescriptionButton.h"

class StartScreenWidget: public QWidget {
    Q_OBJECT
public:
    StartScreenWidget(QWidget* parent = nullptr);
private:
    PlayerControlsWidget* pcw;
    void UpdateScreenPos();
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // STARTSCREEN_H

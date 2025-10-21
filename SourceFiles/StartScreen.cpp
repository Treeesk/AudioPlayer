#include "StartScreen.h"

StartScreenWidget::StartScreenWidget(QWidget* parent): QWidget(parent) {
    pcw = new PlayerControlsWidget(this);
    UpdateScreenPos();
}

void StartScreenWidget::UpdateScreenPos() {
    int centerX = width() / 2;
    pcw->setGeometry(centerX, height() * 3 / 4, pcw->WidthPCW(), 70);
}

void StartScreenWidget::resizeEvent(QResizeEvent* event) {
    UpdateScreenPos();
    QWidget::resizeEvent(event);
}

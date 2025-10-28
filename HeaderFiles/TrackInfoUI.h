#ifndef TRACKINFOUI_H
#define TRACKINFOUI_H
#include <QWidget>
class TrackInfoWidget: public QWidget{
    Q_OBJECT
private:

public:
    TrackInfoWidget(QWidget *parent = nullptr);
    // внутри вызывать функцию отрисовки трека. Мб сделать как слот
};
#endif // TRACKINFOUI_H

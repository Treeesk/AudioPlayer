#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H
#include <QWidget>
#include <QSlider>
#include <QLabel>

class volumeSlider: public QWidget {
    Q_OBJECT
private:
    QSlider* slider;
    QLabel* lowVol;
    QLabel* highVol;
    void UpdateGeometry();
public:
    volumeSlider(const int& width, const int& height, QWidget* parent = nullptr);
public slots:
    void setnewVolume(int value);
signals:
    void newVolume(int vol);
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // VOLUMESLIDER_H

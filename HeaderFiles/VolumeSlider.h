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
    QPixmap baseLowVolPic; // для возможности масштабирования
    QPixmap baseHighVolPic; // для возможности масштабирования
    void UpdateGeometry();
public:
    volumeSlider(QWidget* parent = nullptr);
public slots:
    void setnewVolume(int value);
signals:
    void newVolume(int vol);
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // VOLUMESLIDER_H

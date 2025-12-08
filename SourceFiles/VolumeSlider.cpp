#include "VolumeSlider.h"

volumeSlider::volumeSlider(const int& width, const int& height, QWidget* parent): QWidget(parent) {
    setFixedSize(width, height);

    lowVol = new QLabel(this);
    lowVol->setPixmap(QIcon(":/Resources/Icons/VolumeFirst_cropped_v2.png").pixmap(height, height));
    lowVol->setFixedSize(height, height);

    highVol = new QLabel(this);
    highVol->setPixmap(QIcon(":/Resources/Icons/VolumeThird_cropped_v2.png").pixmap(height, height));
    highVol->setFixedSize(height, height);

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 200);
    slider->setValue(200);
    slider->setSingleStep(2);
    slider->setStyleSheet(R"(
        /* Линия звука(часть которая еще не задействована) */
        QSlider::groove:horizontal {
            border: none;
            height: 4px;
            background: black;
            border-radius: 2px;
            padding: 0px;
            margin: 0px;
        }

        /* Часть которая задействована(белая полоса) */
        QSlider::sub-page:horizontal {
            background: white;
            border-radius: 2px;
            height: 4px;
            margin: 0px;
        }
        /* Ползунок по умолчанию */
        QSlider::handle:horizontal {
            background: transparent;
            border: none;
            width: 0px;
            height: 0px;
            border-radius: 0px;
            margin: 0px;
        }

        /* Ползунок когда слайдер активен (зажат) */
        QSlider::handle:horizontal:pressed {
            background: white;
            border: none;
            width: 10px;
            height: 10px;
            border-radius: 5px;
            margin: -3px 0;
        }
    )");

    connect(slider, &QSlider::sliderReleased, this, [this]() {
        setnewVolume(slider->value());
    });
}

void volumeSlider::setnewVolume(int value) {
    slider->repaint();
    emit newVolume(value / 2);
}

void volumeSlider::UpdateGeometry() {
    int size_icon = 20;
    int space = 5;
    lowVol->setGeometry(0, 0, size_icon, size_icon);
    slider->setGeometry(size_icon + space, 0, width() - size_icon * 2 - space * 2, height());
    highVol->setGeometry(width() - size_icon, 0, size_icon, size_icon);
}

void volumeSlider::resizeEvent(QResizeEvent* event) {
    UpdateGeometry();
    QWidget::resizeEvent(event);
}

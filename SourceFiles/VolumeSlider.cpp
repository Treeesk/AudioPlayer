#include "VolumeSlider.h"


volumeSlider::volumeSlider(QWidget* parent): QWidget(parent) {
    lowVol = new QLabel(this);
    lowIcon = new QIcon(":/Resources/Icons/VolumeFirst_cropped_v2.svg");
    lowVol->setPixmap(lowIcon->pixmap(height(), height()));

    highVol = new QLabel(this);
    highIcon = new QIcon(":/Resources/Icons/VolumeThird_cropped_v2.svg");
    highVol->setPixmap(lowIcon->pixmap(height(), height()));

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
    int size_icon = height();
    int space = 5;
    lowVol->setPixmap(lowIcon->pixmap(size_icon, size_icon));
    lowVol->setGeometry(0, 0, size_icon, size_icon);

    slider->setGeometry(size_icon + space, 0, width() - size_icon * 2 - space * 2, height());

    highVol->setPixmap(highIcon->pixmap(size_icon, size_icon));
    highVol->setGeometry(width() - size_icon, 0, size_icon, size_icon);
}

void volumeSlider::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    UpdateGeometry();
}

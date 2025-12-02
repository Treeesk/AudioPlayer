#include "TrackInfoUI.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QString>

TrackInfoWidget::TrackInfoWidget(const track& trk, const int& _width, const int& _height, QWidget *parent): QWidget(parent) {
    setFixedSize(_width, _height);
    pixmapcover = QPixmap::fromImage(trk.cover);
    artist = trk._artist;
    title = trk._title;
    time = new TrackTime(trk._duration, sizecover, 15, this);
    time->setGeometry(0, sizecover + 5, sizecover, 15);
}

void TrackInfoWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QRect coverRect((width() - sizecover) / 2.0, 0, sizecover, sizecover); // размер 200x200, начало в середина выделенного прос-ва, 0
    painter.setPen(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(coverRect, pixmapcover);

    QFont font("Times New Roman", 16, QFont::DemiBold);
    painter.setFont(font);
    QRect titleRect(0, sizecover + 25, sizecover, 20);
    painter.drawText(titleRect, Qt::AlignCenter | Qt::TextWordWrap, title);

    font.setPointSize(14);
    font.setWeight(QFont::Normal);
    painter.setFont(font);
    QRect artistRect(0, sizecover + 50, sizecover, 20);
    painter.drawText(artistRect, Qt::AlignCenter | Qt::TextWordWrap, artist);
}

void TrackInfoWidget::setTrack(const track& trk) {
    pixmapcover = QPixmap::fromImage(trk.cover);
    artist = trk._artist;
    title = trk._title;
    time->settime(trk._duration);
    update();
}

TrackTime::TrackTime(const int& duration, const int& width, const int& height, QWidget* parent): QWidget(parent){
    setFixedSize(width, height);
    totaltime = duration;
    currenttime = 0;
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, totaltime * 2);
    slider->setValue(0);
    slider->setSingleStep(2);
    slider->setStyleSheet(R"(
        /* Линия трека(часть которая еще не проиграна) */
        QSlider::groove:horizontal {
            border: none;
            height: 4px;
            background: black;
            border-radius: 2px;
            padding: 0px;
            margin: 0px;
        }

        /* Часть которая проиграна(белая полоса) */
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

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TrackTime::changetime);
    connect(slider, &QSlider::sliderReleased, this, [this]() {
        setnewTime(slider->value());
    });
}

void TrackTime::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setPen(Qt::black);
    QString time_of_track;
    if (totaltime % 60 / 10 != 0){
        time_of_track = QString::number(totaltime / 60) + ":" + QString::number(totaltime % 60);
    }
    else {
        time_of_track = QString::number(totaltime / 60) + ":" + "0" + QString::number(totaltime % 60);
    }
    QFontMetrics fm = painter.fontMetrics();
    int heightFont = fm.height(); // высота текста которая может быть при этом размере шрифта
    int widthtotalTime = painter.fontMetrics().horizontalAdvance(time_of_track); // fontMetrics возвращает объект содержащий в пикселях размер шрифта, horizontalAdvance подсчитывает сколько в пикселях текст будет
    QRect timeRect(width() - widthtotalTime, (height() - heightFont) / 2, widthtotalTime, heightFont);
    painter.drawText(timeRect, Qt::AlignLeft | Qt::AlignVCenter, time_of_track);

    QString past_tense;
    if (currenttime % 60 / 10 != 0) {
        past_tense = QString::number(currenttime / 60) + ":" + QString::number(currenttime % 60);
    }
    else {
        past_tense = QString::number(currenttime / 60) + ":" + "0" + QString::number(currenttime % 60);
    }
    int widthpasttense = painter.fontMetrics().horizontalAdvance(past_tense);
    timeRect.setRect(0, (height() - heightFont) / 2, widthpasttense, heightFont);
    painter.drawText(timeRect, Qt::AlignRight | Qt::AlignVCenter, past_tense);
    if (!flag_for_slider_geo) {
        flag_for_slider_geo = true;
        slider->setGeometry(widthpasttense + 5, (height() - heightFont + heightFont / 2) / 2 - 1.5, width() - widthtotalTime - widthpasttense - 10, height());
    }
}

void TrackTime::changetime() {
    if (!seeking) {
        currenttime++;
        slider->setValue(currenttime * 2);
        update();
        if (currenttime >= totaltime) {
            emit endtrack();
        }
    }
}

void TrackTime::setnewTime(int value) {
    seeking = true;
    if (value / 2 >= totaltime) {
        emit endtrack();
        return;
    }
    currenttime = value / 2;
    update();
    emit setTimeTrack(value / 2);
    seeking = false;
}

void TrackTime::settime(const int& duration) {
    totaltime = duration;
    launch = false;
    slider->setRange(0, totaltime * 2);
    slider->setValue(0);
    start();
    update();
}

void TrackTime::start(){
    if (!launch) {
        launch = true;
        currenttime = 0;
    }
    timer->start(1000);
}

void TrackTime::pause() {
    timer->stop();
}

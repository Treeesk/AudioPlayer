#include "TrackInfoUI.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QString>
TrackInfoWidget::TrackInfoWidget(const track& trk, QWidget *parent): QWidget(parent) {
    setFixedSize(sizecover, sizecover + 70);
    pixmapcover = QPixmap::fromImage(trk.cover);
    artist = trk._artist;
    title = trk._title;
    time = new TrackTime(trk._duration, sizecover, 15, this);
    time->setGeometry(0, sizecover + 5, sizecover, 15);
}

void TrackInfoWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    // if (!pixmapcover.isNull()) {
        QRect coverRect((width() - sizecover) / 2.0, 0, sizecover, sizecover); // размер 200x200, начало в 0 0
    // }
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

TrackTime::TrackTime(int duration, int width, int height, QWidget* parent): QWidget(parent){
    setFixedSize(width, height);
    totaltime = duration;
    currenttime = 0;
    _width = width;
    _height = height;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TrackTime::changetime);
}

void TrackTime::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setPen(Qt::black);
    QRect timeRect(0, 0, _width, _height);
    QString time_of_track;
    if (totaltime % 60 / 10 != 0){
        time_of_track = QString::number(totaltime / 60) + ":" + QString::number(totaltime % 60);
    }
    else {
        time_of_track = QString::number(totaltime / 60) + ":" + "0" + QString::number(totaltime % 60);
    }
    painter.drawText(timeRect, Qt::AlignRight, time_of_track);

    QString past_tense;
    if (currenttime % 60 / 10 != 0) {
        past_tense = QString::number(currenttime / 60) + ":" + QString::number(currenttime % 60);
    }
    else {
        past_tense = QString::number(currenttime / 60) + ":" + "0" + QString::number(currenttime % 60);
    }
    painter.drawText(timeRect, Qt::AlignLeft, past_tense);
}

void TrackTime::changetime() {
    currenttime++;
    update();
}

void TrackTime::settime(int duration) {
    totaltime = duration;
    launch = false;
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

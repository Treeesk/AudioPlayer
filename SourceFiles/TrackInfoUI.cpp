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
    QRect coverRect((width() - sizecover) / 2.0, 0, sizecover, sizecover); // размер 200x200, начало в 0 0
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
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TrackTime::changetime);
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
        slider->setGeometry(widthpasttense + 5, (height() - heightFont + heightFont / 2) / 2, width() - widthtotalTime - widthpasttense - 10, height());
    }
}

void TrackTime::changetime() {
    currenttime++;
    update();
    if (currenttime == totaltime) {
        emit endtrack();
    }
}

void TrackTime::settime(const int& duration) {
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

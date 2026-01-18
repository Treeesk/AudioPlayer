#include "TrackInfoUI.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QString>
#include "fontUtils.h"

TrackInfoWidget::TrackInfoWidget(QWidget *parent): TrackInfoBase(parent) {
    setMinimumSize(200, 270);
    time = new TrackTime(this);
}

void TrackInfoWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    if (hasTrack()) {
        painter.drawPixmap(coverRect, getCover());

        QFont font("Times New Roman", fontSizeforTitle, QFont::Normal);
        painter.setFont(font);
        painter.drawText(titleRect, Qt::AlignCenter | Qt::TextWordWrap, getTitle());

        font.setPointSize(fontSizeforArtist);
        font.setWeight(QFont::Normal);
        painter.setFont(font);
        painter.drawText(artistRect, Qt::AlignCenter | Qt::TextWordWrap, getArtist());
    }
}

void TrackInfoWidget::setTrack(const track& trk) {
    changeTrack(trk);
    time->settime(getDuration());
    update();
}

void TrackInfoWidget::initTrack(const track& trk) {
    changeTrack(trk);
    time->initTime(getDuration());
    update();
}

void TrackInfoWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    int fixed = TIMER_HEIGHT + SPACING * 3 + MIN_TEXT;
    int coversize = std::min(width(), height() - fixed); // минимум из доступной ширины, или высоты из которой вычитается фиксированная часть
    coversize = std::max(this->minimumWidth(), coversize); // картинку нужно сделать максимально возможной
    coverRect = QRect(0, 0, coversize, coversize);

    time->setGeometry(0, coverRect.bottom() + SPACING, coversize, TIMER_HEIGHT);

    int textY = time->geometry().bottom() + SPACING; // откуда начнется текст
    int textH = height() - textY; // максимальный размер области для текста

    titleRect = QRect(0, textY, coversize, textH / 2 - SPACING / 2);
    artistRect = QRect(0, textY + textH / 2 + SPACING / 2, coversize, textH / 2 - SPACING / 2);
    fontSizeforTitle = FontUtils::MaxFontSize(getTitle(), titleRect); // высчитывает максимально возможный шрифт для этой области
    fontSizeforArtist = FontUtils::MaxFontSize(getArtist(), artistRect);
}

TrackTime::TrackTime(QWidget* parent): QWidget(parent){
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
    slider->hide(); // скрыть пока трек не инициализирован

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
    int widthtotalTime = fm.horizontalAdvance(time_of_track); // fontMetrics возвращает объект содержащий в пикселях размер шрифта, horizontalAdvance подсчитывает сколько в пикселях текст будет
    QRect timeRect(width() - widthtotalTime, (height() - heightFont) / 2, widthtotalTime, heightFont);
    painter.drawText(timeRect, Qt::AlignLeft | Qt::AlignVCenter, time_of_track);

    QString past_tense;
    if (currenttime % 60 / 10 != 0) {
        past_tense = QString::number(currenttime / 60) + ":" + QString::number(currenttime % 60);
    }
    else {
        past_tense = QString::number(currenttime / 60) + ":" + "0" + QString::number(currenttime % 60);
    }
    int widthpasttense = fm.horizontalAdvance(past_tense);
    timeRect.setRect(0, (height() - heightFont) / 2, widthpasttense, heightFont);
    painter.drawText(timeRect, Qt::AlignRight | Qt::AlignVCenter, past_tense);
}

void TrackTime::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    QString past_tense;
    if (currenttime % 60 / 10 != 0) {
        past_tense = QString::number(currenttime / 60) + ":" + QString::number(currenttime % 60);
    }
    else {
        past_tense = QString::number(currenttime / 60) + ":" + "0" + QString::number(currenttime % 60);
    }
    QFontMetrics fm(font());
    int widthpasttense = fm.horizontalAdvance(past_tense);
    int heightFont = fm.height();

    QString time_of_track;
    if (totaltime % 60 / 10 != 0){
        time_of_track = QString::number(totaltime / 60) + ":" + QString::number(totaltime % 60);
    }
    else {
        time_of_track = QString::number(totaltime / 60) + ":" + "0" + QString::number(totaltime % 60);
    }
    int widthtotalTime = fm.horizontalAdvance(time_of_track);
    slider->setGeometry(widthpasttense + 5, (height() - heightFont + heightFont / 2) / 2 - 1.5, width() - widthtotalTime - widthpasttense - 10, height());
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
    emit SeekChangeIcon();
    if (value / 2 >= totaltime) {
        emit endtrack();
        seeking = false;
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
    timer->stop();
    slider->setRange(0, totaltime * 2);
    slider->setValue(0);
    slider->show();
    start();
    update();
}

void TrackTime::initTime(const int& duration) {
    totaltime = duration;
    launch = false;
    currenttime = 0;
    timer->stop();
    slider->setRange(0, totaltime * 2);
    slider->setValue(0);
    slider->show();
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

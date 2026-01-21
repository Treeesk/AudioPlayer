#include "UI/OpenDirectory.h"
#include <QPainter>
#include <QPainterPath>
#include "Utils/fontUtils.h"
#include <QApplication>

OpenDirectoryButton::OpenDirectoryButton(QWidget* parent): QWidget(parent) {
    OpenDir = new RoundRectPushButton(this);
    OpenDir->setText("Выберите папку");
    connect(OpenDir, &QPushButton::clicked, this, &OpenDirectoryButton::button_clicked);
}

void OpenDirectoryButton::button_clicked() {
    QString directory_name = QFileDialog::getExistingDirectory(
        this,
        tr("Откройте папку"),
        "/Applications",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
    if (directory_name.isEmpty()) {
        return;
    }
    emit dirFound(directory_name.toStdString().c_str());
}

void OpenDirectoryButton::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    OpenDir->setGeometry(0, 0, width(), height());
}

void RoundRectPushButton::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.setRenderHint(QPainter::Antialiasing); // сглаживание краев
    double radius = height() / 2.0;
    QPainterPath path;
    QRectF rect = QRectF(0, 0, width(), height());
    path.addRoundedRect(rect, radius, radius);
    painter.drawPath(path);

    QString _text = this->text();
    int fontSize = FontUtils::MaxFontSize(_text, {int(radius), 0, width() - int(radius) * 2, height()});
    QFont font = QApplication::font();
    font.setPointSize(fontSize);
    font.setWeight(QFont::DemiBold);

    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignCenter, _text);
}

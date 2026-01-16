#include "OpenDirectory.h"
#include <QPainter>
#include <QPainterPath>

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
    emit dirFound(directory_name.toStdString().c_str());
}

void OpenDirectoryButton::resizeEvent(QResizeEvent* event) {
    OpenDir->setGeometry(0, 0, width(), height());
    QWidget::resizeEvent(event);
}

void RoundRectPushButton::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setBrush(Qt::gray);
    painter.setRenderHint(QPainter::Antialiasing); // сглаживание краев
    double radius = height() / 2.0;
    QPainterPath path;
    QRectF rect = QRectF(0, 0, width(), height());
    path.addRoundedRect(rect, radius, radius);
    painter.drawPath(path);

    QString _text = this->text();
    int fontSize = 12;
    QFont font("Times New Roman", fontSize, QFont::Bold);
    int max_width = width() - 10; // отступы по 5px с каждой стороны
    while (fontSize > 6) {
        font.setPointSize(fontSize);
        QFontMetrics fm(font);
        int currentwidth = fm.horizontalAdvance(_text);
        if (currentwidth > max_width) {
            fontSize--;
        }
        else {
            break;
        }
    }
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignCenter, _text);
}

#include "OpenDirectory.h"
#include <QDebug>

OpenDirectoryButton::OpenDirectoryButton(QWidget* parent): QWidget(parent) {
    OpenDir = new QPushButton(this);
    OpenDir->setFixedSize(width(), height());
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

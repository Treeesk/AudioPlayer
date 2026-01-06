#ifndef OPENDIRECTORY_H
#define OPENDIRECTORY_H
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>

class OpenDirectoryButton: public QWidget {
    Q_OBJECT
private:
    QPushButton* OpenDir;
public:
    OpenDirectoryButton(QWidget* parent = nullptr);
public slots:
    void button_clicked();
signals:
    void dirFound(const char* path);
};

#endif // OPENDIRECTORY_H

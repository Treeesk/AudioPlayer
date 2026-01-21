#ifndef OPENDIRECTORY_H
#define OPENDIRECTORY_H
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>

class RoundRectPushButton: public QPushButton {
    Q_OBJECT
public:
    RoundRectPushButton(QWidget* parent = nullptr): QPushButton(parent) {}
protected:
    void paintEvent(QPaintEvent* event);
};

class OpenDirectoryButton: public QWidget {
    Q_OBJECT
private:
    RoundRectPushButton* OpenDir;
public:
    OpenDirectoryButton(QWidget* parent = nullptr);
public slots:
    void button_clicked();
signals:
    void dirFound(const char* path);
    void openDirClicked();
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // OPENDIRECTORY_H

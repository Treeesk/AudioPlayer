#include "mainwindow.h"
#include <QApplication>
#include "DataManager.h"

int main(int argc, char *argv[])
{
    const char* path_to_dir = "/Users/yarchick/AudioPlayerQt/testingmp3mus";
    MusicDataManager manager;
    manager.loadfromdata(path_to_dir);
    QApplication a(argc, argv);
    MainWindow w(&manager);
    w.show();
    return a.exec();
}

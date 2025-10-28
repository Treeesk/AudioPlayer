#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "StartScreen.h"

#include <QPalette>
MainWindow::MainWindow(MusicDataManager* mdm, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(400, 250);
    StartScreenWidget* mainwid = new StartScreenWidget(mdm, this);
    setCentralWidget(mainwid);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(0xD3, 0xD3, 0xD3));
    setPalette(pal);
    setStatusBar(nullptr);
}

MainWindow::~MainWindow()
{
    delete ui;
}

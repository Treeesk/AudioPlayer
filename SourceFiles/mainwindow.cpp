#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <TrackInfoUI.h>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainlayout = new QVBoxLayout(central);
    PlayerControlsWidget* tools = new PlayerControlsWidget(central);
    mainlayout->addWidget(tools);
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connect,&QPushButton::clicked,this,&MainWindow::connectG29);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectG29()
{
    if(!LogiSteeringInitialize(false))
    {
        qDebug() << "error:Init Failed!";
        return;
    }
    qDebug() << "OK!";
}

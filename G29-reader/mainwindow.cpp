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
    int retry = 10;
    DIJOYSTATE2* g29State;
    while(retry--)
    {
        if(LogiSteeringInitialize(false))
        {
            break;
        }
    }
    if(retry == 0)
    {
        qDebug() << "初始化失败!";
        return;
    }
    qDebug() << "初始化成功";
    while(LogiUpdate())
    {
        /*
         * bug:
         * 这个LogiIsConnected函数，如果在运行过程中拔掉usb线，也不会返回false，导致检测不到G29断开连接。
        */
        if(!LogiIsConnected(0))
        {
            qDebug() << "G29已经断开连接!";
            QThread::msleep(1000);
            continue;
        }
        g29State = LogiGetState(0);
        //打印读取到的数据
        qDebug("%d %d %d",
               g29State->lX,
               g29State->lY,
               g29State->lRz);
        QThread::msleep(100);
    }
}

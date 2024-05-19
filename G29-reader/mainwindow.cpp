#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "主窗口运行在线程:" << QThread::currentThread();
    // 实例化对象
    m_g29 = G29::getInterface();
    thread0 = new QThread(this);
    m_tim0 = new QTimer(this);
    /*g29*/
    // 初始化设备
    m_g29->initDevice();
    // 绑定对象和线程
    m_g29->moveToThread(thread0);
    // 开启线程
    thread0->start();
    /*tim0*/
    m_tim0->setInterval(100);
    // 绑定tim和槽函数
    connect(m_tim0, &QTimer::timeout,this,&MainWindow::tim0Handler);
    // 绑定按钮和槽函数
    buttonInit();
    // 绑定信号和槽函数
    connect(this, &MainWindow::updateDevice, m_g29, &G29::update);
    connect(m_g29, &G29::deviceDisconnect, this, [=]()
            {
                showLog("设备断开连接!");
                ui->connectDevice->setEnabled(true);
                m_tim0->stop();});
    connect(this, &MainWindow::destroyed, this, [=]()
            {
                qDebug() << "销毁线程...";
                m_g29->setExitFlag();
                thread0->exit();
                thread0->wait();
                thread0->deleteLater();
                qDebug() << "销毁完毕!";
                qDebug() << "销毁实例化的对象...";
                m_g29->shutDown();
                m_g29->deleteLater();
                qDebug() << "销毁完毕!"; });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonInit()
{
    connect(ui->initSDK, &QPushButton::clicked, this, &MainWindow::SDKInit);
    connect(ui->connectDevice, &QPushButton::clicked, this, &MainWindow::connectDevice);
}

void MainWindow::SDKInit()
{
    if (m_g29->initSDK())
    {
        showLog("[INFO]初始化SDK成功!");
        m_g29->setSDKInitState(true);
        ui->initSDK->setEnabled(false);
    }
    else
    {
        showLog("[ERROR]初始化SKD失败!");
    }
}

void MainWindow::connectDevice()
{
    if (!m_g29->getSDKInitState())
    {
        showLog("[ERROR]SDK未初始化!\n");
        return;
    }
    ui->connectDevice->setEnabled(false);
    emit updateDevice();
    m_tim0->start();
}

void MainWindow::connectServer()
{
}

void MainWindow::showLog(const QString &str)
{
    ui->logBrowser->append(str);
}

void MainWindow::tim0Handler()
{
    //打印数据
    showLog(QString("%1,%2,%3,%4")
                .arg(m_g29->getDirection())
                .arg(m_g29->getPower())
                .arg(m_g29->getBrake())
                .arg(m_g29->getSpeed()));
}

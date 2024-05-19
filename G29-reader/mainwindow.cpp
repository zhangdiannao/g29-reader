#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 实例化对象
    m_g29 = G29::getInterface();
    m_g29->setIsInit(false);
    m_tim0 = new QTimer(this);
    m_tim0->setInterval(100); // 周期为500ms
    // 绑定按钮到槽函数
    connect(ui->connect, &QPushButton::clicked, this, &MainWindow::connectDevice);
    // 绑定定时器到槽函数
    connect(m_tim0, &QTimer::timeout, this, &MainWindow::tim0Handler);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectDevice()
{
    if (!m_g29->isInit())
    {
        // 初始化
        if (!m_g29->init())
        {
            qDebug() << "初始化失败!";
            return;
        }
        m_g29->setIsInit(true);
        qDebug() << "初始化成功!";
    }
    m_tim0->start();
}

void MainWindow::tim0Handler()
{
    if (!m_g29->isConnected())
    {
        qDebug() << "设备断开连接!";
        m_tim0->stop();
        return;
    }
    // 更新数据
    if (!m_g29->update())
    {
        qDebug() << "更新数据失败!";
        return;
    }
    // 打印数据
    // qDebug() << "方向盘" << m_g29->m_data->lX
    //          << "油门" << m_g29->m_data->lY
    //          << "刹车" << m_g29->m_data->lRz;
    /*
    "刹车" << m_g29->m_data->lRz
    "方向盘" << m_g29->m_data->lX
    "油门" << m_g29->m_data->lY
    "升档" << m_g29->m_data->rgbButtons[4]
    "降档" << m_g29->m_data->rgbButtons[5]
    */
}

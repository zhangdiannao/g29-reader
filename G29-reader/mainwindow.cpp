#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //实例化对象
    m_g29 = new G29(this);
    m_tim0 = new QTimer(this);

    //配置定时器
    m_tim0->setInterval(33);//一秒执行30次



    //连接信号和槽函数
    connect(ui->button_init,&QPushButton::clicked,this,&MainWindow::deviceInit);
    connect(m_tim0,&QTimer::timeout,this,&MainWindow::tim0Handler);


    /*窗口销毁后动作:
     * 这个只能Lambda表达式，用mainwindow的槽函数执行会报错
    */
    connect(this,&MainWindow::destroyed,this,[=]()
    {
        //手动释放G29相关资源
        LogiSteeringShutdown();
        m_g29->setSDKInitState(false);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deviceInit()
{
    //初始化SDK，SDK只需要初始化一次
    if(!m_g29->getSDKInitState())
    {
        if(!LogiSteeringInitializeWithWindow(true,(HWND)winId()))
        {
            log("SDK初始化失败!");
            return;
        }
        m_g29->setSDKInitState(true);
    }

    log("SDK初始化成功!");

    if(!LogiIsConnected(0))
    {
        log("设备未连接!");
        //手动释放G29相关资源
        LogiSteeringShutdown();
        m_g29->setSDKInitState(false);
        return;
    }
    DIJOYSTATE2* temp = LogiGetState(0);
    if(!temp)
    {
        log("获取设备数据地址失败!");
        return;
    }
    m_g29->setDataAddr(temp);
    log("获取设备数据地址成功!");
    log("设备连接成功!");
    m_tim0->start();
}

void MainWindow::log(const QString &str)
{
    ui->logBrowser->append(str);
}

void MainWindow::tim0Handler()
{
    //检查连接状态
    if(!LogiIsConnected(0))
    {
        log("设备断开连接!");
        //手动释放G29相关资源
        LogiSteeringShutdown();
        m_g29->setSDKInitState(false);
        m_tim0->stop();
        return;
    }
    //更新控制器数据
    LogiUpdate();
    //更新数据到UI界面
    ui->val_direction->setValue(m_g29->getDirection());
    ui->val_power->setValue(m_g29->getPower());
    ui->val_brake->setValue(m_g29->getBrake());
    ui->val_speed->setValue(m_g29->getSpeed());
#if 0
    //打印控制器数据
    log(QString("%1 %2 %3 %4")
        .arg(m_g29->getDirection())
        .arg(m_g29->getPower())
        .arg(m_g29->getBrake())
        .arg(m_g29->getSpeed()));
#endif

}

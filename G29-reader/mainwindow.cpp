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
    m_effect = new QSoundEffect(this);

    //配置定时器
    m_tim0->setInterval(33);//一秒执行30次

    //配置提示音
    m_effect->setSource(QUrl::fromLocalFile(":/res/audio0.wav"));
    m_effect->setLoopCount(1);
    m_effect->setVolume(100);

    //连接信号和槽函数
    connect(ui->button_init,&QPushButton::clicked,this,&MainWindow::deviceInit);
    connect(m_tim0,&QTimer::timeout,this,&MainWindow::tim0Handler);
    connect(ui->button_disconnect_device,&QPushButton::clicked,this,&MainWindow::deviceDeInit);
    connect(ui->button_exit,&QPushButton::clicked,this,[=]()
    {
        deviceDeInit();
        /*这里用destory()线程不会退出*/
        this->close();
    });


    /*窗口销毁后动作:
     * 这个只能Lambda表达式，用mainwindow的槽函数执行会报错
    */
    connect(this,&MainWindow::destroyed,this,[=]()
    {
        /*这里用deviceDeInit()会导致崩溃*/
        //手动释放G29相关资源
        LogiSteeringShutdown();
        m_g29->setSDKInitState(false);
        m_g29->setSpringState(false);
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
        deviceDeInit();
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

void MainWindow::deviceDeInit()
{
    //手动释放G29相关资源
    LogiSteeringShutdown();
    m_g29->setSDKInitState(false);
    m_g29->setSpringState(false);
    m_tim0->stop();
    log("释放G29相关资源 OK!");
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
        deviceDeInit();
        return;
    }
    //更新控制器数据
    LogiUpdate();
    //配置中心弹簧(回正力度)
    if(!m_g29->getSprinfState())
    {
        if(LogiPlaySpringForce(0,0,60,60))
        {
            log("配置回正力度成功!");
            m_g29->setSpringState(true);
        }else
        {
            log("配置回正力度失败!");
        }
    }

    /*按钮检测*/
    if(m_g29->getspeedUpButtonState())
    {
        m_g29->m_up.pressTime++;
        m_g29->m_up.lastState = true;
    }
    if(m_g29->getspeedDownButtonState())
    {
        m_g29->m_down.pressTime++;
        m_g29->m_down.lastState = true;
    }
    //松手检测
    if(m_g29->getspeedUpButtonState() == false && m_g29->m_up.lastState == true)
    {
        m_g29->speedUp();
        log("升档!");
        m_effect->play();
        m_g29->m_up.lastState = false;
    }
    if(m_g29->getspeedDownButtonState() == false && m_g29->m_down.lastState == true)
    {
        m_g29->speedDown();
        log("降档!");
        m_effect->play();
        m_g29->m_down.lastState = false;
    }
    /*********/
    //更新数据到UI界面
    ui->val_direction->setValue(m_g29->getDirection());
    ui->val_power->setValue(m_g29->getPower());
    ui->val_brake->setValue(m_g29->getBrake());
    ui->val_speed->setValue(m_g29->getSpeed());
#if 1
    //打印控制器数据
    log(QString("%1 %2 %3 %4")
        .arg(m_g29->getDirection())
        .arg(m_g29->getPower())
        .arg(m_g29->getBrake())
        .arg(m_g29->getSpeed()));
#endif

}

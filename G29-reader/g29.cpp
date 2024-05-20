#include "g29.h"

// 初始化单例对象
G29 *G29::m_interface = NULL;

G29::G29(QObject *parent)
    : QObject{parent}
{
    m_speed = 1;
    m_exitFlag = 0;
}

G29 *G29::getInterface()
{
    if (m_interface == NULL)
    {
        m_interface = new G29();
    }
    return m_interface;
}

bool G29::initSDK()
{
    return LogiSteeringInitialize(false);
}

void G29::setSDKInitState(bool state)
{
    m_SDKInitState = state;
}

bool G29::getSDKInitState()
{
    return m_SDKInitState;
}

void G29::initDataAddr()
{
    m_data = LogiGetStateENGINES(0);
}

bool G29::isDtaAddrIsNULL()
{
    return (m_data == NULL) ? true : false;
}

bool G29::isConnected()
{
    return LogiIsConnected(0);
}

void G29::update()
{
    qDebug() << "更新设备数据运行在线程:" << QThread::currentThread();
    while (true)
    {
        if (m_exitFlag || !isConnected())
        {
            qDebug() << "设备断开连接!";
            emit deviceDisconnect();
            return;
        }
        LogiUpdate();
        QThread::msleep(10);
    }
}

int G29::getDirection()
{

    return m_data->lX;
}

int G29::getPower()
{
    return m_data->lY;
}

int G29::getBrake()
{
    return m_data->lRz;
}

char G29::getSpeed()
{
    return m_speed;
}

void G29::speedUp()
{
    m_speed += 1;
    if (m_speed == 3)
    {
        m_speed = 2;
    }
}

void G29::speedDown()
{
    m_speed -= 1;
    if (m_speed == -1)
    {
        m_speed = 0;
    }
}

void G29::setExitFlag()
{
    m_exitFlag = 1;
}

void G29::shutDown()
{
    LogiSteeringShutdown();
}

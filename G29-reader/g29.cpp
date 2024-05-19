#include "g29.h"

G29 *G29::m_interface = nullptr;

G29::G29(QObject *parent)
    : QObject{parent}
{
}

// 返回单例对象
G29 *G29::getInterface()
{
    if (m_interface == NULL)
    {
        m_interface = new G29();
    }
    return m_interface;
}

// 初始化控制器
bool G29::init()
{
    if (!LogiSteeringInitialize(false))
    {
        return false;
    }
    m_data = LogiGetState(0);
    return true;
}

void G29::setIsInit(bool state)
{
    m_isInit = state;
}

// 获取控制器初始化状态
bool G29::isInit()
{
    return m_isInit;
}

// 更新控制器数据
bool G29::update()
{
    if (!LogiUpdate())
    {
        return false;
    }
    return true;
}

bool G29::isConnected()
{
    return LogiIsConnected(0);
}

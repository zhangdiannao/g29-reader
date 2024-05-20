#include "g29.h"

G29::G29(QObject *parent)
    : QObject{parent}
{
    m_SDKInitState = false;
    m_dataAddr = NULL;
    m_speed = 1;
}

bool G29::init()
{
    return false;
}

void G29::setSDKInitState(bool state)
{
    m_SDKInitState = state;
}

bool G29::getSDKInitState()
{
    return m_SDKInitState;
}

void G29::setDataAddr(DIJOYSTATE2 *addr)
{
    m_dataAddr = addr;
}

bool G29::isDataAddrNULL()
{
    return (m_dataAddr == NULL) ? true : false;
}

LONG G29::getDirection()
{
    return m_dataAddr->lX;
}

LONG G29::getPower()
{
    return -(m_dataAddr->lY);
}

LONG G29::getBrake()
{
    return -(m_dataAddr->lRz);
}

int G29::getSpeed()
{
    return m_speed;
}

void G29::speedUp()
{
    m_speed += 1;
    if (m_speed > MAX_SPEED)
    {
        m_speed = MAX_SPEED;
    }
}

void G29::speedDown()
{
    m_speed -= 1;
    if (m_speed < MIN_SPEED)
    {
        m_speed = MIN_SPEED;
    }
}

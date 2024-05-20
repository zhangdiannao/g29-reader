#ifndef G29_H
#define G29_H

#include <QObject>
#include <QDebug>
#include <LogitechSteeringWheelLib.h>

#define MAX_SPEED 2
#define MIN_SPEED 0

class G29 : public QObject
{
    Q_OBJECT
public:
    explicit G29(QObject *parent = nullptr);
    // 初始化控制器
    bool init();
    // 设置SDK初始化状态
    void setSDKInitState(bool state);
    // 获取SDK初始化状态
    bool getSDKInitState();
    // 设置设备数据地址
    void setDataAddr(DIJOYSTATE2 *addr);
    // 设备数据地址是否为空
    bool isDataAddrNULL();
    // 获取方向原始数据
    LONG getDirection();
    // 获取油门原始数据
    LONG getPower();
    // 获取刹车原始数据
    LONG getBrake();
    // 获取挡位
    int getSpeed();
    // 升挡
    void speedUp();
    // 降挡
    void speedDown();
signals:
private:
    bool m_SDKInitState;
    DIJOYSTATE2 *m_dataAddr;
    int m_speed; // 当前挡位
};

#endif // G29_H

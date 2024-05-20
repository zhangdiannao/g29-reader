#ifndef G29_H
#define G29_H

#include <QObject>
#include <LogitechSteeringWheelLib.h>
#include <QThread>
#include <QDebug>

class G29 : public QObject
{
    Q_OBJECT
public:
    // 单例模式获取接口
    static G29 *getInterface();
    // 初始化SDK
    bool initSDK();
    // 设置初始化状态
    void setSDKInitState(bool state);
    // 获取初始化状态
    bool getSDKInitState();
    // 初始化设备数据地址
    void initDataAddr();
    // 设备数据地址是否未空
    bool isDtaAddrIsNULL();
    // 获取设备连接状态
    bool isConnected();
    // 更新设备数据
    void update();
    // 获取方向盘角度原始数据
    int getDirection();
    // 获取油门原始数据
    int getPower();
    // 获取刹车原始数据
    int getBrake();
    // 获取挡位数据
    char getSpeed();
    // 升档
    void speedUp();
    // 降档
    void speedDown();
    // 置位推出标志
    void setExitFlag();
    // 销毁资源
    void shutDown();
signals:
    void deviceDisconnect();

private:
    // 私有化构造函数保证单例
    explicit G29(QObject *parent = nullptr);
    // 单例对象
    static G29 *m_interface;
    // 初始化状态
    bool m_SDKInitState;
    // 设备数据地址
    DIJOYSTATE2ENGINES *m_data;
    // 挡位
    char m_speed;
    // 线程退出标志
    bool m_exitFlag;
};
#endif // G29_H

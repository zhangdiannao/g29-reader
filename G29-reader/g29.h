#ifndef G29_H
#define G29_H

#include <QObject>
#include <QDebug>
#include <LogitechSteeringWheelLib.h>

/*
这个类用单例模式
*/

class G29 : public QObject
{
    Q_OBJECT
public:
    /*暂时把这个属性放在public里，之后会改到private*/
    DIJOYSTATE2 *m_data;

    // 返回单例对象
    static G29 *getInterface();

    explicit G29(QObject *parent = nullptr);
    // 初始化控制器
    bool init();
    // 设置控制器初始化状态
    void setIsInit(bool state);
    // 获取控制器初始化状态
    bool isInit();
    // 更新控制器数据
    bool update();
    // 获取连接状态
    bool isConnected();
signals:

private:
    static G29 *m_interface;
    bool m_isInit;
};

#endif // G29_H

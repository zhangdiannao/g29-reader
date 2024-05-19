#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <LogitechSteeringWheelLib.h>
#include <QTimer>
#include <g29.h>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // 绑定按钮与槽函数
    void buttonInit();
    // 初始化SDK
    void SDKInit();
    // 连接设备
    void connectDevice();
    // 连接服务器
    void connectServer();
    // 输出日志
    void showLog(const QString &str);
    // tim0周期事件
    void tim0Handler();
signals:
    void updateDevice();

private:
    Ui::MainWindow *ui;
    G29 *m_g29;
    QThread *thread0;
    QTimer *m_tim0;
};
#endif // MAINWINDOW_H

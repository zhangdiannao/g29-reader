#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <g29.h>
#include <LogitechSteeringWheelLib.h>
#include <QTimer>
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

    // 初始化设备
    void deviceInit();
    // 输出日志
    void log(const QString &str);
    //定时器0周期事件
    void tim0Handler();

private:
    Ui::MainWindow *ui;
    G29 *m_g29;
    DIJOYSTATE2 *m_data;
    QTimer *m_tim0;
};
#endif // MAINWINDOW_H

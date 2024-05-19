#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <LogitechSteeringWheelLib.h>
#include <g29.h>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //连接设备
    void connectDevice();
    //定时器0周期事件
    void tim0Handler();

private:
    Ui::MainWindow *ui;
    G29* m_g29;
    QTimer* m_tim0;
};
#endif // MAINWINDOW_H

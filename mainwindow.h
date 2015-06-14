#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>    /*添加qt线程的头文件*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
/*
 * 添加界面主线程的信号槽函数，用来实现与线程的信号对接
*/
private slots:
    void lcdnumValSet(int num);

private:
    Ui::MainWindow *ui;
};


/*
 * 添加线程类的声明
*/
class MyThread: public QThread
{
    Q_OBJECT
public:
    MyThread();
    void run();
signals:
    void send(int num);
};



#endif // MAINWINDOW_H

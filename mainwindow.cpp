#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
 * 加入gpio操作的相关头文件
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/gpiofd.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*
     * 添加线程声明，绑定信号和信号槽，随后启动线程
    */
    MyThread *myth = new MyThread;
    connect(myth,SIGNAL(send(int)),this,SLOT(lcdnumValSet(int)));
    myth->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::lcdnumValSet(int num)
{
    ui->lcdNumber->display(num);
}


/*
 * 添加线程类的相关代码，在此线程中启动gpio读取功能
*/
MyThread::MyThread()
{

}
void MyThread::run()
{
    int inum = 10;
    int     fd;
    fd_set  fdset;
    uint8_t value;
    int     gpioInNum  = 159;   /*key0*/
    struct  timeval     timeout	= { .tv_sec	= 60,.tv_usec = 0 };

    fd = gpiofd(gpioInNum, 0, GPIO_FLAG_TRIG_FALL | GPIO_FLAG_TRIG_RISE);
    if (fd <= 0) {
        perror("gpiofd create error!");
    }

    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);
    /*
     * 使用中断方式获取 GPIO 状态时，需要使用 select 函数配合程序编写
     * 等待30s，无反应则线程退出
     */
    while(true){
        if (select(fd + 1, &fdset, NULL, NULL, &timeout) == 1) {
            gpiofd_read(fd, &value);
            printf("value : %d\n", value);
            inum++;
             /*
              * 向主线程发生线程信号，发送后延时0.5s，做消抖处理
              */
            emit send(inum);
            usleep(500*1000);

        }else{
            break;
        }
    }
    ::close(fd);
    exec();
}


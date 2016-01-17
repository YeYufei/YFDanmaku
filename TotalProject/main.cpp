#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);     //创建一个Qt应用
    MainWindow w;       //在应用空间里构建这个类
    w.show();           //这个类如果有窗体的话 show
    return a.exec();    //返回一个Qt应用的exec函数 开启事件循环
}

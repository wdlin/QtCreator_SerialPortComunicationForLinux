﻿#include "MainWindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.init();//自定义的初始化函数
    w.show();

    return a.exec();
}

//#define QT_NO_CAST_FROM_ASCII 1
#include "mainwindow.h"

#include <QApplication>
#include <QThread>


int main(int argc, char *argv[])
{
    QThread::currentThread()->setObjectName("Qt Mainthread");

    //QString s("abc");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

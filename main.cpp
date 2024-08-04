//#define QT_NO_CAST_FROM_ASCII 1
#include "mainwindow.h"

#include <QApplication>
#include <QThread>


int main(int argc, char *argv[])
{
    QThread::currentThread()->setObjectName("Qt Mainthread");

    QString s("abc");
    auto s2 = s.toUtf8();
    qDebug() << s;

    auto v = QVector<QString>();
    auto v2 = QSet<QString>({
        "abc", "def"
    });

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

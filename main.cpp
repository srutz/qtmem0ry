//#define QT_NO_CAST_FROM_ASCII 1
#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QThread>
#include <QTimer>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <processthreadsapi.h>
#elif defined(Q_OS_LINUX)
#include <pthread.h>
#elif defined(Q_OS_MACOS)
#include <pthread.h>
#endif


int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SetThreadDescription(GetCurrentThread(), QString::fromUtf8(name).toStdWString().c_str());
#elif defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    pthread_setname_np(pthread_self(), "Qt Mainthread!");
#endif

    QApplication a(argc, argv);
    MainWindow w;

    QTimer::singleShot(0, [&] () {
        char *key = getenv("PEXELSAPIKEY");
        if (key == nullptr || strlen(key) == 0) {
            QMessageBox messageBox;
            messageBox.setWindowTitle("PEXELSAPIKEY missing");
            messageBox.setText("Set the environment-variable PEXELSAPIKEY to a valid apikey from Pexels.");
            messageBox.setIcon(QMessageBox::Information);
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.setDefaultButton(QMessageBox::Ok);
            messageBox.exec();
            QApplication::exit(1);
        } else {
            w.show();
        }

    });

    return a.exec();
}

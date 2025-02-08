//#define QT_NO_CAST_FROM_ASCII 1
#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QStandardPaths>


#if defined(Q_OS_LINUX)
#include <pthread.h>
#elif defined(Q_OS_MACOS)
#include <pthread.h>
#endif


int main(int argc, char *argv[])
{
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    pthread_setname_np(pthread_self(), "Qt Mainthread!");
#endif

    QApplication a(argc, argv);
    MainWindow w;

    QTimer::singleShot(0, [&] () {
        char *key = getenv("PEXELSAPIKEY");
        if (key == nullptr || strlen(key) == 0) {
            auto path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/pexelsapikey.txt";
            QFile file(path);
            qDebug() << "reading apikey from " << path;
            if (file.open(QIODevice::ReadOnly)) {
                auto data = file.readAll();
                qDebug() << "data" << QString::fromUtf8(data);
                auto firstLine = QString::fromUtf8(data).trimmed();
                if (!firstLine.isEmpty()) {
                    key = strdup(firstLine.toStdString().c_str());
                }
            }
        }

        if (key == nullptr || strlen(key) == 0) {
            QMessageBox messageBox;
            messageBox.setWindowTitle("PEXELSAPIKEY missing");
            messageBox.setText("Set the environment-variable PEXELSAPIKEY to a valid apikey from Pexels. "
                "Alternatively put the key into a file called pexelsapikey.txt in your HOME directory.");
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

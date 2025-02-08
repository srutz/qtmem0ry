#include <iostream>
#include <QMap>
#include <QMainWindow>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include "util.h"

Util::Util() {}


QStatusBar* Util::findStatusBar() {
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        QMainWindow *mainWin = qobject_cast<QMainWindow *>(widget);
        if (mainWin) {
            return mainWin->statusBar();
        }
    }
    return nullptr;
}


QByteArray Util::loadFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open the file:" << file.errorString();
        return QByteArray();
    }
    QByteArray fileData = file.readAll();
    file.close();
    return fileData;
}

int Util::showMessage(const QString &message)
{
    QMessageBox messageBox(QApplication::activeWindow());
    messageBox.setWindowTitle("Message");
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Ok);
    auto result = messageBox.exec();
    return result;
}

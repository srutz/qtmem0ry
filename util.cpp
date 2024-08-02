#include <iostream>
#include <QMap>
#include <QMainWindow>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <curl/curl.h>
#include "util.h"



Util::Util() {}




/*
 * Download an url to a file synchronously
 *
 */
ErrorResponse Util::downloadUrl(const QString &url, const QMap<QString,QString> &headers) {
    QByteArray buffer;
    CURL *curl = curl_easy_init();
    if (!curl) {
        return {
            .code = -1,
                .message = "Curl not available"
        };
    }
    struct curl_slist *header_list = NULL;
    for (auto it = headers.constBegin(); it != headers.constEnd(); ++it) {
        QString header = it.key() + ": " + it.value();
        std::string header_std = header.toStdString();
        header_list = curl_slist_append(header_list, header_std.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
    curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void *buffer, size_t size, size_t nmemb, void *userp) -> size_t {
        size_t s = size * nmemb;
        QByteArray* mem = static_cast<QByteArray*>(userp);
        mem->append(static_cast<char*>(buffer), s);
        return s;
    });

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        return {
            .code = -1,
            .message = "Failed to write data"
        };

        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    curl_slist_free_all(header_list);
    curl_easy_cleanup(curl);
    return {
        .code = 200,
        .message = "",
        .data = QByteArray(buffer),
    };
}

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
    QMessageBox messageBox;
    messageBox.setWindowTitle("Message");
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Ok);
    auto result = messageBox.exec();
    return result;
}

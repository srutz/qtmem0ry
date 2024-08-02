#ifndef DOWNLOADHANDLER_H
#define DOWNLOADHANDLER_H

#include <QObject>
#include <QDir>
#include <QDebug>


class DownloadHandler : public QObject {
    Q_OBJECT

public:
    explicit DownloadHandler(QObject *parent = nullptr);
    ~DownloadHandler();

public slots:
    void download();

signals:
    void imageCompleted(int i, int n, const QByteArray &data);
    void completed();

private:

};
#endif // DOWNLOADHANDLER_H

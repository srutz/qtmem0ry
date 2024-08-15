#ifndef DOWNLOADHANDLER_H
#define DOWNLOADHANDLER_H

#include <QObject>
#include <QDir>
#include <QDebug>


class DownloadHandler : public QObject {
    Q_OBJECT

public:
    explicit DownloadHandler(int count, QObject *parent = nullptr);
    ~DownloadHandler();

public slots:
    void download();

signals:
    void imageCompleted(int i, int n, const QByteArray &data, QString filename);
    void completed();

private:
    int m_count;

};
#endif // DOWNLOADHANDLER_H

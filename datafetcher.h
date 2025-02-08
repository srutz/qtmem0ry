#ifndef DATAFETCHER_H
#define DATAFETCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QMap>
#include <optional>

using namespace std;

/*
 * Generic Data Fetcher
 */


class DataFetcher : public QObject
{
    Q_OBJECT

public:
    /* options for the fetch
    */
    struct FetchOptions {
        QString url;
        QMap<QString,QString> headers;
        optional<QString> httpMethod; // POST, GET, DELETE, PUT etc
        optional<QByteArray> data; // POST / PUT upload-data
    };

    /* return struct for sync fetch
     */
    struct FetchResults {
        QByteArray data;
        int code;
        optional<QString> error;
    };

    explicit DataFetcher(QObject *parent = nullptr);
    void fetch(const FetchOptions &options);

    /* download sync and block current thread */
    static FetchResults downloadSync(const FetchOptions &options);


signals:
    void responseReceived(const QByteArray &data, int httpStatusCode);
    void error(const QString &message, int httpStatusCode);

private slots:
    void handleNetworkResponse(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};


#endif // DATAFETCHER_H

#include "datafetcher.h"
#include "raiiguard.h"
#include <QEventLoop>
#include <QObject>

using namespace std;

DataFetcher::DataFetcher(QObject *parent)
    : QObject(parent)
    , manager(new QNetworkAccessManager(this))
{
    connect(manager, &QNetworkAccessManager::finished, this, &DataFetcher::handleNetworkResponse);
}

void DataFetcher::fetch(const FetchOptions &options)
{
    QUrl url(options.url);
    if (!url.isValid()) {
        emit error("Invalid URL", -1);
        return;
    }
    QNetworkRequest request(url);
    qDebug() << "Fetching URL:" << url.toString();
    for (auto it = options.headers.begin(); it != options.headers.end(); ++it) {
        auto key = it.key();
        auto value = it.value();
        request.setRawHeader(key.toUtf8(), value.toUtf8());
    }
    if (options.httpMethod.value_or("GET") == "POST") {
        //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        manager->post(request, options.data.value_or(QByteArray()));
    } else {
        manager->get(request);
    }
}

void DataFetcher::handleNetworkResponse(QNetworkReply *reply)
{
    /* make sure reply is deleted under all circumstances */
    RAIIGuard guard([reply] { reply->deleteLater(); });
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (reply->error() != QNetworkReply::NoError) {
        emit error(reply->errorString(), httpStatusCode);
        return;
    }
    auto data = reply->readAll();
    qDebug() << "got data" << data.size() << "bytes" << " for url " << reply->url().toString();
    emit responseReceived(data, httpStatusCode);
}

/* download sync and block current thread 
 * this is achieved by a custom temporary event loop
 */

DataFetcher::FetchResults DataFetcher::downloadSync(const FetchOptions &options) {
    QEventLoop loop;
    auto fetcher = new DataFetcher();
    QByteArray result;
    QString error;
    int code = -1;
    connect(fetcher, &DataFetcher::responseReceived, [&loop,&result,fetcher,&code](const QByteArray &data, int responseCode) {
        //qDebug() << "got data" << data.size() << "bytes" << ": utf8=" << QString::fromUtf8(data);
        result.append(data);
        fetcher->deleteLater();
        loop.quit();
        code = responseCode;
    });
    connect(fetcher, &DataFetcher::error, [&loop,&error,fetcher,&code](const QString &message, int responseCode) {
        //qDebug() << "error " << message;
        fetcher->deleteLater();
        loop.quit();
        error += message;
        code = responseCode;
    });
    fetcher->fetch(options);
    qDebug() << "waiting for download to complete";
    loop.exec();
    return FetchResults({
        .data = result,
        .code = code,
        .error = error.isEmpty() ? std::nullopt : make_optional(error)
    });
}

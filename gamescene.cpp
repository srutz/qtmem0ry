
#include <QDebug>
#include <QString>
#include <QGraphicsItem>
#include <QResizeEvent>
#include <QDir>


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <QDebug>
#include <QThread>
#include <QVBoxLayout>
#include <QPropertyAnimation>

#include "gamescene.h"
#include "util.h"
#include "downloadhandler.h"



GameScene::GameScene(QWidget *parent)
    : QGraphicsView{parent}
{
    qDebug() << "GameScene()";
    this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->scene = new QGraphicsScene(this);
    this->scene->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->initResources();
    this->setScene(scene);
    this->layoutScene();
}

GameScene::~GameScene()
{
    qDebug() << "~GameScene()";
}


void GameScene::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // base class handler
    auto parent = this->parentWidget();
    double scale = std::min(parent->width(), parent->height()) / 1000.0;
    qInfo() << "resize" << event->size() << ", scale=" << scale;
    //this->setTransform(QTransform::fromScale(scale, scale));
    this->layoutScene();
}

void GameScene::initResources()
{
    QThread *downloadThread = new QThread();
    downloadThread->setObjectName("downloader thread");
    DownloadHandler *handler = new DownloadHandler();
    handler->moveToThread(downloadThread);

    QObject::connect(downloadThread, &QThread::started, handler, &DownloadHandler::download);
    QObject::connect(handler, &DownloadHandler::completed, this, [=]() {
        handler->deleteLater();
        this->layoutScene();
    });

    QObject::connect(handler, &DownloadHandler::imageCompleted, this, [=](int i, int n, QByteArray data) {
        qDebug() << "image " << i << " downloaded " << QThread::currentThread()->objectName();
        Util::findStatusBar()->showMessage(QString("Retrieved Image %1 of %2").arg(i + 1).arg(n), 5000);
        QPixmap pixmap;
        bool success = pixmap.loadFromData(data);
        if (!success) {
            qDebug() << "failed to load image " << i << " into pixmap";
            return;
        }
        int size = qMin(pixmap.width(), pixmap.height());
        QRect cropRect;
        if (pixmap.width() > pixmap.height()) {
            int offsetX = (pixmap.width() - size) / 2;
            cropRect.setRect(offsetX, 0, size, size);
        } else {
            int offsetY = (pixmap.height() - size) / 2;
            cropRect.setRect(0, offsetY, size, size);
        }
        QPixmap squaredPixmap = pixmap.copy(cropRect);
        //int w2 = squaredPixmap.width();
        //int h2 = squaredPixmap.height();
        //qDebug() << "w2=" << w2 << ", h2=" << h2 << "   crop=" << cropRect;
        auto pi = new QGraphicsPixmapItem(squaredPixmap);
        auto scaledSize = 64.0;
        auto scale = qMin(scaledSize / squaredPixmap.width(), scaledSize / squaredPixmap.height());
        pi->setScale(scale);
        pi->setPos(0, 0);
        scene->addItem(pi);
    });
    QObject::connect(downloadThread, &QThread::finished, downloadThread, &QObject::deleteLater);
    downloadThread->start();
}


void GameScene::layoutScene()
{
    auto aw = this->size().width();
    auto ah = this->size().height();
    auto padding = 16;
    auto dist = 10;
    //auto tw = ((aw - padding * 2) - (7 - dist)) / 8;
    auto tw = 64;

    qDebug() << "layoutScene(): aw=" << aw << ", ah=" << ah;
    auto cards = std::vector<QGraphicsItem*>();
    for(auto item : this->scene->items()) {
        cards.push_back(item);
    }
    auto size = static_cast<int>(sqrt(cards.size()));
    auto y = padding;
    for (int row = 0; row < size; row++) {
        auto x = padding;
        for (int col = 0; col < size; col++) {
            auto item = cards[row * size + col];
            /*
            auto brect = item->boundingRect();
            auto scaleX = tw / brect.width();
            auto scaleY = tw / brect.height();
            auto scale= qMin(scaleX, scaleY);
            item->setScale(scale);
            */
            item->setPos(x, y);
            x += tw + dist;
        }
        y += tw + dist;
    }
}




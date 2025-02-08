
#include <cmath>
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
#include <QTimer>

#include <cassert>
#include <random>
#include <algorithm>
#include <vector>


#include "downloadhandler.h"
#include "util.h"
#include "memorypanel.h"
#include "memorycard.h"


MemoryPanel::MemoryPanel(QWidget *parent):
    QWidget(parent),
    m_size(4),
    m_gameState(GameState::STOPPED),
    m_gameCount(0)
{
}

void MemoryPanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // base class handler
    this->layoutChildren();
}


void MemoryPanel::setupChildren()
{
    auto backgroundPixmap = QPixmap(":/images/background.png");

    QThread *downloadThread = new QThread();
    downloadThread->setObjectName("downloader thread");
    DownloadHandler *handler = new DownloadHandler(m_size * m_size / 2);
    handler->moveToThread(downloadThread);

    QObject::connect(downloadThread, &QThread::started, handler, &DownloadHandler::download);
    QObject::connect(handler, &DownloadHandler::completed, this, [=]() {
        handler->deleteLater();
        this->layoutChildren();
        this->updateStatusMessage();
    });

    QObject::connect(handler, &DownloadHandler::imageCompleted, this, [=](int i, int n, QByteArray data, QString filename) {
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
        auto squaredPixmap = pixmap.copy(cropRect);
        for (auto j = 0; j < 2; j++) {
            auto key = QString::asprintf("key%03d", i);
            auto card = new MemoryCard(this, key, j, squaredPixmap, backgroundPixmap, this);
            QFileInfo info(filename);
            card->setFilename(info.baseName());
            card->setPosition(QPoint(-1000, -1000));
            card->setFixedSize(SIZE_4, SIZE_4);
            card->show();
        }
    });
    QObject::connect(downloadThread, &QThread::finished, downloadThread, &QObject::deleteLater);
    downloadThread->start();

}

void MemoryPanel::updateStatusMessage()
{
    QString message = "";
    switch (m_gameState) {
    case STARTED:
        message += QString("Game started. Hit #%1. Miss #%2").arg(
            QString::number(m_stats.hits()),
            QString::number(m_stats.misses()));
        break;
    case STOPPED:
        message += "Game not started";
        break;
    case OVER:
        message += QString("Game Over. Hit #%1. Miss #%2. Total moves #%3").arg(
            QString::number(m_stats.hits()),
            QString::number(m_stats.misses()),
            QString::number(m_stats.moves()))
            ;
        break;
    }
    Util::findStatusBar()->showMessage(message);
}


void MemoryPanel::layoutChildren()
{
    auto aw = this->size().width();
    auto ah = this->size().height();
    auto cards = std::vector<MemoryCard*>();
    for(auto child : children()) {
        auto card = qobject_cast<MemoryCard*>(child);
        if (card) {
            cards.push_back(card);
        }
    }

    auto size = m_size;

    if (m_gameState != GameState::STARTED) {
        auto i = 0;
        for (auto card : cards) {
            card->show();
            auto x = i % 2 == 0 ? - 200 : aw + 200;
            auto y = i % size < size / 2 ? - 200 : ah + 200;
            if (m_gameCount <= 0) {
                card->setPosition(QPoint(x, y));
            } else {
                card->setPositionA(QPoint(x, y), 500);
            }
            i++;
        }
        return;
    }
    auto padding = 0;
    auto dist = 10;
    int tw = 0;
    switch (size) {
    case 4: tw = SIZE_4; break;
    case 6: tw = SIZE_6; break;
    }

    auto th = tw;
    auto wantedw = size * tw + (size - 1) * dist;
    auto wantedh = size * th + (size - 1) * dist;
    auto x0 = (int) floor(.5 * (aw - wantedw));
    auto y0 = (int) floor(.5 * (ah - wantedh));
    //qDebug() << "layoutScene(): aw=" << aw << ", ah=" << ah << ", x0" << x0 << ", size=" << cards.size();
    auto y = padding + y0;
    for (int i = 0; i < cards.size(); i++) {
        cards[i]->hide();
    }
    for (int row = 0; row < size; row++) {
        auto x = padding + x0;
        for (int col = 0; col < size; col++) {
            auto card = cards[m_order[row * size + col]];
            card->setFixedSize(tw, th);
            card->setPositionA(QPoint(x, y), 500);
            card->show();
            x += tw + dist;
        }
        y += tw + dist;
    }
}

const Stats& MemoryPanel::stats() const {
    return m_stats;
}

GameState MemoryPanel::gameState() const
{
    return m_gameState;
}

void MemoryPanel::setGameState(GameState gameState)
{
    if (this->m_gameState == gameState) {
        return;
    }
    this->m_gameState = gameState;
    if (this->m_gameState == GameState::STARTED) {
        m_gameCount++;
        /* cleanup */
        std::random_device rd;
        std::default_random_engine eng(rd());
        m_order.resize(m_size * m_size);
        for (auto i = 0, n = m_size * m_size; i < n; i++) {
            m_order[i] = i;
        }
        std::shuffle(m_order.begin(), m_order.end(), eng);
        for(auto child : children()) {
            auto card = qobject_cast<MemoryCard*>(child);
            if (card) {
                card->setPresent(true);
                if (card->flipState() != FlipState::FLIPPED) {
                    card->flip();
                }
            }
        }
        m_stats.reset();
        m_seen.clear();
    } else if (this->m_gameState == GameState::OVER) {
        Util::showMessage(QString("Game completed. Number of moves: %1.").arg(m_stats.moves()));
    }
    layoutChildren();
    updateStatusMessage();
}

void MemoryPanel::cardClicked(MemoryCard *card)
{
    switch (m_gameState) {
    case STOPPED:
        Util::showMessage("The Game is not started.");
        break;
    case OVER:
        Util::showMessage("The Game is over, start a new game.");
        break;
    case STARTED:
        auto visibleCards = getVisibleCards();
        /* we have 2 cards unflipped, then do check and remove them if they match */
        if (visibleCards.size() >= 2) {
            return;
        } else {
            if (card->flipState() == FlipState::FLIPPED) {
                card->flip([=]() {
                    checkForMatch();
                });
            }
        }
        break;
    }
}

std::vector<MemoryCard*> MemoryPanel::getVisibleCards() {
    std::vector<MemoryCard*> unflippedCards;
    for(auto child : children()) {
        auto card = qobject_cast<MemoryCard*>(child);
        if (card && card->present() && Util::isAnyEquals(card->flipState(), { FlipState::UNFLIPPED, FlipState::UNFLIPPING })) {
            unflippedCards.push_back(card);
        }
    }
    return unflippedCards;
}

void MemoryPanel::checkForMatch() {
    auto visibleCards = getVisibleCards();
    if (visibleCards.size() > 2) {
        // wrong invariant, only show 2 cards max
        assert(false);
    } else if (visibleCards.size() == 1) {
        m_firstCard = visibleCards[0];
    } else if (visibleCards.size() == 2) {
        auto card1 = visibleCards[0];
        auto card2 = visibleCards[1];
        m_stats.setMoves(m_stats.moves() + 1);
        if (card1->key() == card2->key()) {
            card1->setPresent(false);
            card2->setPresent(false);

            auto presentCount = 0;
            auto children_ = children();
            for(auto child : children_) {
                auto card = qobject_cast<MemoryCard*>(child);
                if (card && card->present()) {
                    presentCount++;
                }
            }
            if (presentCount == 0) {
                setGameState(GameState::OVER);
            }
            m_stats.setHits(m_stats.hits() + 1);
        } else {
            if (m_firstCard) {
                // chance to uncover a card missed here
                auto otherCardKey = QPair<QString,int>(m_firstCard->key(), m_firstCard->index() == 0 ? 1 : 0);
                if (m_seen.contains(otherCardKey)) {
                    m_stats.setSeenMisses(m_stats.seenMisses() + 1);
                }
            }
            card1->flip();
            card2->flip();
            m_stats.setMisses(m_stats.misses() + 1);
        }
        for (const auto &card : visibleCards) {
            m_seen.insert(QPair<QString,int>(card->key(), card->index()));
        }
        updateStatusMessage();
    }
}

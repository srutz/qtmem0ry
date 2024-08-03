#ifndef MEMORYPANEL_H
#define MEMORYPANEL_H

#include <QWidget>

enum GameState {
    STOPPED,
    STARTED,
    OVER,
};

class Stats : public QObject
{
    Q_OBJECT
public:

    Stats() {
        this->reset();
    }

    void reset() {
        this->setHits(0);
        this->setMisses(0);
        this->setSeenMisses(0);
        this->setMoves(0);
    }

    int hits() const { return this->m_hits; }
    void setHits(int hits) {
        if (hits == m_hits) return;
        this->m_hits = hits;
        emit changed(*this);
    }

    int misses() const { return this->m_misses; }
    void setMisses(int misses) {
        if (misses == m_misses) return;
        this->m_misses = misses;
        emit changed(*this);
    }

    int seenMisses() const { return this->m_seenMisses; }
    void setSeenMisses(int seenMisses) {
        if (seenMisses == m_seenMisses) return;
        this->m_seenMisses = seenMisses;
        emit changed(*this);
    }

    int moves() const { return this->m_moves; }
    void setMoves(int moves) {
        if (moves == m_moves) return;
        this->m_moves = moves;
        emit changed(*this);
    }

signals:
    void changed(const Stats& stats);

private:
    int m_hits;
    int m_misses;
    int m_seenMisses;
    int m_moves;

};


class MemoryCard;

class MemoryPanel : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryPanel(QWidget *parent = nullptr);

    void setupChildren();
    void layoutChildren();
    void updateStatusMessage();

    const Stats& stats() const;
    const Stats* statsPointer() const { return &m_stats; }

    GameState gameState() const;
    void setGameState(GameState);

    void cardClicked(MemoryCard *card);
    std::vector<MemoryCard *> getVisibleCards();
    void checkForMatch();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    std::vector<int> m_order;
    GameState m_gameState;
    QSet<QString> m_seen;
    Stats m_stats;
    MemoryCard *m_firstCard; // remember the first card for stats

signals:
};

#endif // MEMORYPANEL_H

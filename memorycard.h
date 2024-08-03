#ifndef MEMORYCARD_H
#define MEMORYCARD_H

#include "animatedwidget.h"

#include <functional>
#include <QWidget>
#include <QPixmap>
#include <QPropertyAnimation>

#include "memorypanel.h"

class MemoryPanel;

enum FlipState { UNFLIPPED = 1, FLIPPING = 2,  FLIPPED = 3, UNFLIPPING = 4};

class MemoryCard : public AnimatedWidget
{
    Q_OBJECT
public:
    Q_PROPERTY(double angle READ angle WRITE setAngle NOTIFY angleChanged)

    explicit MemoryCard(MemoryPanel *panel, const QString &key, int index, const QPixmap &pixmap, const QPixmap &backPixmap, QWidget *parent = nullptr);

    const QString& key() const;
    int index() const;

    FlipState flipState() const;
    QString flipStateAsString() const;
    void flip(std::function<void()> onCompleteCallback = nullptr);

    bool present() const;
    void setPresent(bool);

    double angle() const;
    void setAngle(double angle);
    void setAngleA(double angle, std::function<void()> onCompleteCallback = nullptr); // animated

signals:
    void angleChanged(double angle);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;


private:
    MemoryPanel *m_panel;
    QString m_key;
    int m_index;
    QPixmap m_pixmap;
    QPixmap m_backPixmap;
    QTransform m_transform;

    bool m_present;
    FlipState m_flipState;
    double m_angle;
    QPropertyAnimation *angleAnimation;
};

#endif // MEMORYCARD_H

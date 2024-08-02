#ifndef ANIMATEDWIDGET_H
#define ANIMATEDWIDGET_H

#include <QPropertyAnimation>
#include <QWidget>

class AnimatedWidget : public QWidget
{
    Q_OBJECT
public:
    Q_PROPERTY(QPoint position READ position WRITE setPosition NOTIFY positionChanged)

    explicit AnimatedWidget(QWidget *parent = nullptr);
    QPoint position() const;
    void setPosition(const QPoint position);
    void setPositionA(const QPoint position, int durationMs = 350); // animated

signals:
    void positionChanged(const QPoint point);

private:
    QPoint m_position;
    QPropertyAnimation *positionAnimation;
};

#endif // ANIMATEDWIDGET_H

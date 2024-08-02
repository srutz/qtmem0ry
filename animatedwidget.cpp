#include "animatedwidget.h"

AnimatedWidget::AnimatedWidget(QWidget *parent)
    : m_position(QPoint(0, 0)), positionAnimation(nullptr), QWidget{parent}
{}

QPoint AnimatedWidget::position() const
{
    return this->m_position;
}

void AnimatedWidget::setPosition(const QPoint position)
{
    //qDebug() << "setPosition. oldValue=" << m_position << " to " << position << (m_position == position);
    if (m_position == position) {
        return;
    }
    m_position = position;
    this->move(m_position);
    emit positionChanged(m_position);
}

void AnimatedWidget::setPositionA(const QPoint position, int durationMs)
{
    if (positionAnimation) {
        positionAnimation->stop();
        delete positionAnimation;
    }
    auto anim = new QPropertyAnimation(this, "position");
    this->positionAnimation = anim;
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    anim->setDuration(durationMs);
    anim->setStartValue(m_position);
    anim->setEndValue(position);
    anim->start();
}

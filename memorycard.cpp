#include <cmath>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include "memorycard.h"
#include "memorypanel.h"

MemoryCard::MemoryCard(MemoryPanel *panel, const QString &key, int index, const QPixmap &pixmap, const QPixmap &backPixmap, QWidget *parent) :
    AnimatedWidget(parent),
    m_panel(panel),
    m_key(key),
    m_index(index),
    m_pixmap(pixmap),
    m_backPixmap(backPixmap),
    m_present(true),
    m_flipState(FlipState::FLIPPED),
    m_angle(180),
    angleAnimation(nullptr)
{
    m_transform.reset();

    this->setCursor(Qt::PointingHandCursor);
    setPosition(QPoint(-1000, -1000));
    //qDebug() << "the map: " << pixmap.width() << " x " << pixmap.height();
}

const QString& MemoryCard::key() const {
    return m_key;
}

int MemoryCard::index() const {
    return m_index;
}

FlipState MemoryCard::flipState() const
{
    return this->m_flipState;
}

QString MemoryCard::flipStateAsString() const {
    switch (this->m_flipState) {
    case FLIPPED:
        return "FLIPPED";
    case UNFLIPPED:
        return "UNFLIPPED";
    case FLIPPING:
        return "FLIPPING";
    case UNFLIPPING:
        return "UNFLIPPING";
    default:
        return "NA";
    }
}

bool MemoryCard::present() const {
    return m_present;
}

void MemoryCard::setPresent(bool present) {
    if (this->m_present == present) {
        return;
    }
    this->m_present = present;
    this->update();
}

double MemoryCard::angle() const
{
    return this->m_angle;
}

void MemoryCard::setAngle(double angle)
{
    if (this->m_angle == angle) {
        return;
    }
    this->m_angle = angle;
    emit angleChanged(angle);
}

void MemoryCard::setAngleA(double angle, std::function<void()> onCompleteCallback) {
    if (angleAnimation) {
        angleAnimation->stop();
        delete angleAnimation;
    }
    auto anim = new QPropertyAnimation(this, "angle");
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    anim->setDuration(350);
    anim->setStartValue(m_angle);
    anim->setEndValue(angle);
    anim->start();
    connect(anim, &QPropertyAnimation::valueChanged, this, [=] {
        this->update();
    });
    connect(anim, &QPropertyAnimation::finished, this, [=] {
        if (onCompleteCallback) {
            onCompleteCallback();
        }
    });
    this->angleAnimation = anim;
}

void MemoryCard::mousePressEvent(QMouseEvent *event)
{
    m_panel->cardClicked(this);
    QWidget::mousePressEvent(event);
}

void MemoryCard::flip(std::function<void()>  onCompleteCallback)
{
    switch (m_flipState) {
    case FlipState::UNFLIPPED:
    case FlipState::UNFLIPPING:
        m_flipState = FlipState::FLIPPING;
        setAngleA(180, [=] {
            m_flipState = FlipState::FLIPPED;
            if (onCompleteCallback) {
                onCompleteCallback();
            }
        });
        break;
    case FlipState::FLIPPED:
    case FlipState::FLIPPING:
        m_flipState = FlipState::UNFLIPPING;
        setAngleA(0, [=] {
            m_flipState = FlipState::UNFLIPPED;
            if (onCompleteCallback) {
                onCompleteCallback();
            }
        });
        break;
    }
}

void MemoryCard::paintEvent(QPaintEvent* event)
{
    {
        QPainter painter(this);

        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        if (m_present) {
            // Calculate the center point
            QPoint center(width() / 2, height() / 2);

            m_transform.reset();
            m_transform.rotate(m_angle, Qt::YAxis);

            // Apply the transformation
            painter.translate(center);
            painter.setTransform(m_transform, true);
            painter.translate(-center);

            // Draw the pixmap centered in the widget

            QRect pixRect = this->rect();
            auto bw = 12;
            pixRect.setRect(pixRect.x() + bw, pixRect.y() + bw, pixRect.width() - 2 * bw, pixRect.height() - 2 * bw);

            painter.setPen(QColor::fromRgb(255, 255, 255, 255));
            painter.setBrush(QColor::fromRgb(255, 255, 255, 255));
            painter.drawRoundedRect(this->rect(), 8, 8);
            QPoint topLeft((width() - m_pixmap.width()) / 2, (height() - m_pixmap.height()) / 2);
            auto p = ((m_angle) <= 90.0 || (m_angle) >= 270.0) ? m_pixmap : m_backPixmap;
            painter.drawPixmap(pixRect, p);
        }
    }

    const auto DEBUG_STATE = false;
    if (DEBUG_STATE) {
        QPainter painter(this);
        QPen pen;
        auto y = 16;
        painter.fillRect(QRect(0, 0, 200, 80), QBrush(QColor(40, 40, 40, 150)));
        pen.setColor(Qt::white);
        painter.setPen(pen);
        //painter.setFont(QFont("Arial", 12));
        auto h = 16;
        auto printLine = [&](const QString &text) {
            painter.drawText(0, y, text);
            y += h;
        };
        printLine("key=" + this->key() + ", index=" + QString::number(this->index()));
        printLine("flipState=" + this->flipStateAsString());
        printLine(QString("present=") + (present() ? "true" : "false"));
    }
}



#include "lane.h"

Lane::Lane(QWidget *parent) : QWidget(parent), lane(1)
{
    if (parent)
    {
        setMinimumSize(parent->width() * 0.5, parent->height() * 0.5);
        // setMaximumSize(parent->width() * 0.6, parent->height() * 0.4);
    }
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/sports.png");
    digital_path = QDir::cleanPath(digital_path);
    pixmap = QPixmap(digital_path);
    // pixmap_original = QPixmap(digital_path);
    // pixmap = pixmap_original.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    pixmap = pixmap.scaled(this->width() * 0.4 , height() * 0.5, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, [this]() {
        m_leftDashOffset = int(m_leftDashOffset + 1) % 30;
        m_rightDashOffset = int(m_rightDashOffset + 1) % 30;
        update();
    });
    animationTimer->start(80);
    setters();
    set_lane(82);
}

void Lane::setters()
{
    leftOpacityAnimation = new QPropertyAnimation(this, "leftOpacity");
    rightOpacityAnimation = new QPropertyAnimation(this, "rightOpacity");

    laneGradient = QLinearGradient(0, height() * 0.9, 0, 0);
    laneGradient.setColorAt(0.0, QColor(20, 20, 20, 20));
    laneGradient.setColorAt(0.4, QColor(150, 150, 150, 150));
    laneGradient.setColorAt(0.5, QColor(150, 150, 150, 150));
    laneGradient.setColorAt(1.0, QColor(20, 20, 20, 1));

    redGradient = QLinearGradient(0, height() * 0.9, 0, 0);
    redGradient.setColorAt(0.0, QColor(50, 5, 0, 50));
    redGradient.setColorAt(0.4, QColor(200, 30, 0, 150));
    redGradient.setColorAt(0.6, QColor(200, 30, 0, 150));
    redGradient.setColorAt(1.0, QColor(50, 5, 0, 50));

    leftGrayPen = QPen(QBrush(laneGradient), 5, Qt::DashLine);
    leftRedPen = QPen(QBrush(redGradient), 5, Qt::DashLine);
    rightGrayPen = QPen(QBrush(laneGradient), 5, Qt::DashLine);
    rightRedPen = QPen(QBrush(redGradient), 5, Qt::DashLine);
}

void Lane::showNoLanePopup()
{
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/alert.png");
    digital_path = QDir::cleanPath(digital_path);
    if (!popup)
    {
        popup = new QWidget(this);
        popup->setAttribute(Qt::WA_DeleteOnClose);
        popup->setStyleSheet("background-color: transparent;");
        QHBoxLayout *layout = new QHBoxLayout(popup);
        QLabel *iconLabel = new QLabel(popup);
        QPixmap warningIcon(digital_path);
        iconLabel->setPixmap(warningIcon.scaled(22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QLabel *textLabel = new QLabel(" Take manual control", popup);
        textLabel->setStyleSheet("color: gray; font-size: 16px;");
        layout->addWidget(iconLabel, 0, Qt::AlignTop | Qt::AlignCenter);
        layout->addWidget(textLabel, 0, Qt::AlignCenter);
        popup->setLayout(layout);
        popup->adjustSize();
        popup->move((width() - popup->width()) / 2, height() - popup->height());

        opacityEffect = new QGraphicsOpacityEffect(popup);
        opacityEffect->setOpacity(0.0);
        popup->setGraphicsEffect(opacityEffect);
        popup->show();

        QPropertyAnimation *fadeIn = new QPropertyAnimation(opacityEffect, "opacity");
        fadeIn->setDuration(300);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        fadeIn->setEasingCurve(QEasingCurve::InOutSine);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

// void Lane::resizeEvent(QResizeEvent *event)
// {
//     if (!pixmap_original.isNull()) {
//         pixmap = pixmap_original.scaled(event->size(),
//                                         Qt::KeepAspectRatio,
//                                         Qt::SmoothTransformation);
//         update();
//     }

//     QWidget::resizeEvent(event); // call base implementation
// }


void Lane::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QRect rect = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, pixmap.size(), this->rect());
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(rect, pixmap);

    int laneBottomLeft = width() * 0.2;
    int laneBottomRight = width() * 0.8;
    int laneTopLeft = width() * 0.4;
    int laneTopRight = width() * 0.6;

    leftGrayPen.setDashOffset(m_leftDashOffset);
    painter.setPen(leftGrayPen);
    painter.setOpacity(1.0 - m_leftOpacity);
    painter.drawLine(QPoint(laneBottomLeft, height()), QPoint(laneTopLeft, height() * 0.1));

    leftRedPen.setDashOffset(m_leftDashOffset);
    painter.setPen(leftRedPen);
    painter.setOpacity(m_leftOpacity);
    painter.drawLine(QPoint(laneBottomLeft, height()), QPoint(laneTopLeft, height() * 0.1));

    rightGrayPen.setDashOffset(m_rightDashOffset);
    painter.setPen(rightGrayPen);
    painter.setOpacity(1.0 - m_rightOpacity);
    painter.drawLine(QPoint(laneBottomRight, height()), QPoint(laneTopRight, height() * 0.1));

    rightRedPen.setDashOffset(m_rightDashOffset);
    painter.setPen(rightRedPen);
    painter.setOpacity(m_rightOpacity);
    painter.drawLine(QPoint(laneBottomRight, height()), QPoint(laneTopRight, height() * 0.1));
}

void Lane::set_lane(int i)
{
    if (lane && lane == i)
        return;
    lane = i;
    leftOpacityAnimation->stop();
    rightOpacityAnimation->stop();
    qreal targetLeftOpacity = 0.0;
    qreal targetRightOpacity = 0.0;
    if (i == 76)
        targetLeftOpacity = 1.0;
    else if (i == 82)
        targetRightOpacity = 1.0;

    if (m_leftOpacity != targetLeftOpacity)
    {
        leftOpacityAnimation->setDuration(400);
        leftOpacityAnimation->setStartValue(m_leftOpacity);
        leftOpacityAnimation->setEndValue(targetLeftOpacity);
        leftOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        leftOpacityAnimation->start();
    }
    if (m_rightOpacity != targetRightOpacity)
    {
        rightOpacityAnimation->setDuration(400);
        rightOpacityAnimation->setStartValue(m_rightOpacity);
        rightOpacityAnimation->setEndValue(targetRightOpacity);
        rightOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        rightOpacityAnimation->start();
    }
    if (lane == 0)
        showNoLanePopup();
    else if (popup)
    {
        QPropertyAnimation *fadeOut = new QPropertyAnimation(opacityEffect, "opacity");
        fadeOut->setDuration(400);
        fadeOut->setStartValue(opacityEffect->opacity());
        fadeOut->setEndValue(0.0);
        fadeOut->setEasingCurve(QEasingCurve::InOutSine);
        connect(fadeOut, &QPropertyAnimation::finished, this, [this]() {
            if (popup)
            {
                popup->close();
                popup->deleteLater();
                popup = nullptr;
            }
        });
        fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    }
    update();
}

qreal Lane::leftOpacity() const
{
    return m_leftOpacity;
}

void Lane::setLeftOpacity(qreal opacity)
{
    m_leftOpacity = opacity;
    update();
    emit leftOpacityChanged();
}

qreal Lane::rightOpacity() const
{
    return m_rightOpacity;
}

qreal Lane::leftDashOffset() const
{
    return m_leftDashOffset;
}

qreal Lane::rightDashOffset() const
{
    return m_rightDashOffset;
}

void Lane::setRightOpacity(qreal opacity)
{
    m_rightOpacity = opacity;
    update();
    emit rightOpacityChanged();
}

QGraphicsOpacityEffect *Lane::get_opacityEffect()
{
    return opacityEffect;
}

int Lane::get_lane()
{
    return lane;
}

QWidget *Lane::get_popup()
{
    return popup;
}

Lane::~Lane()
{
    if (leftOpacityAnimation) {
        leftOpacityAnimation->stop();
        delete leftOpacityAnimation;
        leftOpacityAnimation = nullptr;
    }
    if (rightOpacityAnimation) {
        rightOpacityAnimation->stop();
        delete rightOpacityAnimation;
        rightOpacityAnimation = nullptr;
    }
    if (animationTimer)
    {
        animationTimer->stop();
    }
    if (popup)
    {
        popup->close();
        popup->deleteLater();
        popup = nullptr;
    }
    std::cout << "Remove Lane" << std::endl;
}
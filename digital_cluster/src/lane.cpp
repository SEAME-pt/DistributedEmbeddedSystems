#include "lane.h"

Lane::Lane(QWidget *parent) : QWidget(parent)
{
    if (parent) {
        setMinimumSize(parent->width() * 0.4, parent->height() * 0.4); 
        setMaximumSize(parent->width() * 0.4, parent->height() * 0.4);
    }
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/sports.png"); 
    digital_path = QDir::cleanPath(digital_path);
    pixmap = QPixmap(digital_path);
    pixmap = pixmap.scaled(this->width() * 0.4, height() * 0.7, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, [this]() {
        m_leftDashOffset = std::fmod(m_leftDashOffset + 1.0, 30.0);
        m_rightDashOffset = std::fmod(m_rightDashOffset + 1.0, 30.0);
        update();
    });
    animationTimer->start(80);
    setters();
    set_lane(2);
}

void Lane::setters() 
{
    leftOpacityAnimation = new QPropertyAnimation(this, "leftOpacity");
    leftOpacityAnimation->setDuration(400);
    leftOpacityAnimation->setStartValue(0.0);
    leftOpacityAnimation->setEndValue(1.0);
    leftOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);

    rightOpacityAnimation = new QPropertyAnimation(this, "rightOpacity");
    rightOpacityAnimation->setDuration(400);
    rightOpacityAnimation->setStartValue(0.0);
    rightOpacityAnimation->setEndValue(1.0);
    rightOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);

    laneGradient = QLinearGradient(0, height() * 0.9, 0, 0);
    laneGradient.setColorAt(0.0, QColor(20, 20, 20, 50)); 
    laneGradient.setColorAt(0.4, QColor(100, 100, 100, 175)); 
    laneGradient.setColorAt(0.6, QColor(100, 100, 100, 175));
    laneGradient.setColorAt(1.0, QColor(20, 20, 20, 50)); 
    
    redGradient = QLinearGradient(0, height() * 0.9, 0, 0);
    redGradient.setColorAt(0.0, QColor(50, 5, 0, 50)); 
    redGradient.setColorAt(0.4, QColor(180, 20, 0, 150)); 
    redGradient.setColorAt(0.6, QColor(180, 20, 0, 150)); 
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
    if (!popup) {
        popup = new QWidget(this);
        popup->setAttribute(Qt::WA_DeleteOnClose);
        QHBoxLayout* layout = new QHBoxLayout(popup);
        QLabel* iconLabel = new QLabel(popup);
        QPixmap warningIcon(digital_path);
        iconLabel->setPixmap(warningIcon.scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
        QLabel* textLabel = new QLabel(" Please take manual control", popup);
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

        QPropertyAnimation* fadeIn = new QPropertyAnimation(opacityEffect, "opacity");
        fadeIn->setDuration(300);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        fadeIn->setEasingCurve(QEasingCurve::InOutSine);
        connect(fadeIn, &QPropertyAnimation::finished, this, [this]() {
            std::cout << "Popup fade-in completed" << std::endl;
        });
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void Lane::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignCenter,
        pixmap.size(),
        this->rect()
    );
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

void   Lane::set_lane(int i)
{
    if (lane == i) 
        return ; 
    lane = i;

    QPropertyAnimation *leftFadeToGray = nullptr;
    QPropertyAnimation *rightFadeToGray = nullptr;

    if (m_leftOpacity > 0.0) {
        leftOpacityAnimation->stop();
        leftFadeToGray = new QPropertyAnimation(this, "leftOpacity");
        leftFadeToGray->setDuration(400);
        leftFadeToGray->setStartValue(m_leftOpacity);
        leftFadeToGray->setEndValue(0.0);
        leftFadeToGray->setEasingCurve(QEasingCurve::InOutSine);
    }

    if (m_rightOpacity > 0.0) {
        rightOpacityAnimation->stop();
        rightFadeToGray = new QPropertyAnimation(this, "rightOpacity");
        rightFadeToGray->setDuration(400);
        rightFadeToGray->setStartValue(m_rightOpacity);
        rightFadeToGray->setEndValue(0.0);
        rightFadeToGray->setEasingCurve(QEasingCurve::InOutSine);
    }

    if (lane == 0) {
        if (leftFadeToGray) {
            leftFadeToGray->start(QAbstractAnimation::DeleteWhenStopped);
        }
        if (rightFadeToGray) {
            rightFadeToGray->start(QAbstractAnimation::DeleteWhenStopped);
        }
        showNoLanePopup();
    } else {
        if (i == 76) {
            if (leftOpacityAnimation->state() != QAbstractAnimation::Running) {
                leftOpacityAnimation->start();
            }
            if (rightFadeToGray) {
                rightFadeToGray->start(QAbstractAnimation::DeleteWhenStopped);
            }
        } else if (i == 82) {
            if (rightOpacityAnimation->state() != QAbstractAnimation::Running) {
                rightOpacityAnimation->start();
            }
            if (leftFadeToGray) {
                leftFadeToGray->start(QAbstractAnimation::DeleteWhenStopped);
            }
        } else {
            if (leftFadeToGray) {
                leftFadeToGray->start(QAbstractAnimation::DeleteWhenStopped);
            }
            if (rightFadeToGray) {
                rightFadeToGray->start(QAbstractAnimation::DeleteWhenStopped);
            }
        }
        if (popup) {
            QPropertyAnimation *fadeOut = new QPropertyAnimation(opacityEffect, "opacity");
            fadeOut->setDuration(400);
            fadeOut->setStartValue(opacityEffect->opacity());
            fadeOut->setEndValue(0.0);
            fadeOut->setEasingCurve(QEasingCurve::InOutSine);
            connect(fadeOut, &QPropertyAnimation::finished, this, [this]() {
                if (popup) {
                    popup->close();
                    popup->deleteLater();
                    popup = nullptr;
                }
            });
            fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
        }
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

void Lane::setRightOpacity(qreal opacity) 
{
    m_rightOpacity = opacity;
    update();
    emit rightOpacityChanged();
}

qreal Lane::leftDashOffset() const 
{ 
    return m_leftDashOffset; 
}

void Lane::setLeftDashOffset(qreal offset)
{
    if (qFuzzyCompare(m_leftDashOffset, offset)) return;
    m_leftDashOffset = offset;
    emit leftDashOffsetChanged();
}

qreal Lane::rightDashOffset() const 
{ 
    return m_rightDashOffset; 
}

void Lane::setRightDashOffset(qreal offset)
{
    if (qFuzzyCompare(m_rightDashOffset, offset)) return;
    m_rightDashOffset = offset;
    emit rightDashOffsetChanged();
}

int   Lane::get_lane()
{
    return lane;
}

Lane::~Lane()
{
    delete leftOpacityAnimation;
    delete rightOpacityAnimation;
    delete animationTimer;
    if (popup) {
        popup->close();
        popup->deleteLater();
        popup = nullptr;
    }
}
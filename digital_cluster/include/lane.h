#ifndef LANE_H
#define LANE_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QDir>
#include <QStyle>
#include <QColor>
#include <QLabel>
#include <iostream>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <cmath>

class Lane: public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal leftDashOffset READ leftDashOffset WRITE setLeftDashOffset NOTIFY leftDashOffsetChanged)
    Q_PROPERTY(qreal rightDashOffset READ rightDashOffset WRITE setRightDashOffset NOTIFY rightDashOffsetChanged)
    Q_PROPERTY(qreal leftOpacity READ leftOpacity WRITE setLeftOpacity NOTIFY leftOpacityChanged)
    Q_PROPERTY(qreal rightOpacity READ rightOpacity WRITE setRightOpacity NOTIFY rightOpacityChanged)

private:
    QWidget *popup = nullptr;
    int lane;
    QPen leftGrayPen;
    QPen rightGrayPen;
    QPen leftRedPen;
    QPen rightRedPen;

    qreal m_leftDashOffset = 0.0;
    qreal m_rightDashOffset = 0.0;
    QPropertyAnimation *leftAnimation;
    QPropertyAnimation *rightAnimation;
    qreal m_leftOpacity = 1.0;
    qreal m_rightOpacity = 1.0;
    QPropertyAnimation *leftOpacityAnimation;
    QPropertyAnimation *rightOpacityAnimation;
    QTimer *animationTimer = nullptr;
    QLinearGradient redGradient;
    QLinearGradient laneGradient;
    QPixmap pixmap;
    QGraphicsOpacityEffect *opacityEffect;
    
public:
    Lane(QWidget *parent = nullptr);
    ~Lane();
    void showNoLanePopup();
    int   get_lane();
    void   set_lane(int i);

    qreal rightDashOffset() const;
    qreal leftDashOffset() const;
    void setLeftDashOffset(qreal offset);
    void setRightDashOffset(qreal offset);

    qreal rightOpacity() const;
    qreal leftOpacity() const;
    void setLeftOpacity(qreal offset);
    void setRightOpacity(qreal offset);
    void setters();

protected:
    void paintEvent(QPaintEvent *) override;

signals:
    void leftDashOffsetChanged();
    void rightDashOffsetChanged();
    void leftOpacityChanged();
    void rightOpacityChanged();

};

#endif
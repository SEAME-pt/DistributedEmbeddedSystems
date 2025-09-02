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
#include <QResizeEvent>


class Lane: public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal leftOpacity READ leftOpacity WRITE setLeftOpacity NOTIFY leftOpacityChanged)
    Q_PROPERTY(qreal rightOpacity READ rightOpacity WRITE setRightOpacity NOTIFY rightOpacityChanged)

protected:
    QWidget *popup = nullptr;
    int lane;
    QPen leftGrayPen;
    QPen rightGrayPen;
    QPen leftRedPen;
    QPen rightRedPen;
    QPixmap pixmap_original; 
    
    qreal m_leftDashOffset = 0.0;
    qreal m_rightDashOffset = 0.0;
    qreal m_leftOpacity = 1.0;
    qreal m_rightOpacity = 1.0;
    QPropertyAnimation *leftOpacityAnimation = nullptr;
    QPropertyAnimation *rightOpacityAnimation = nullptr;
    QTimer *animationTimer = nullptr;
    QLinearGradient redGradient;
    QLinearGradient laneGradient;
    QPixmap pixmap;
    QGraphicsOpacityEffect *opacityEffect;
    void paintEvent(QPaintEvent *) override;
    // void resizeEvent(QResizeEvent *event) override;

    
public:
    Lane(QWidget *parent = nullptr);
    ~Lane();
    void showNoLanePopup();
    int   get_lane();
    void   set_lane(int i);

    qreal rightOpacity() const;
    qreal leftOpacity() const;
    void setLeftOpacity(qreal offset);
    void setRightOpacity(qreal offset);
    void setters();
    QWidget*   get_popup();
    qreal rightDashOffset() const;
    qreal leftDashOffset() const;
    QGraphicsOpacityEffect *get_opacityEffect();

signals:
    void leftDashOffsetChanged();
    void rightDashOffsetChanged();
    void leftOpacityChanged();
    void rightOpacityChanged();

};

#endif
#ifndef LANE_H
#define LANE_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QTimer>
#include <QMessageBox>

class Lane: public QWidget {
    Q_OBJECT

private:
    QVector<QPoint> leftLane;  // Holds detected left lane points
    QVector<QPoint> rightLane; // Holds detected right lane points
    QMessageBox *pop_up;

public:
    Lane(QWidget *parent = nullptr);
    ~Lane();
    void updateLaneData(const QVector<QPoint> &newLeftLane, const QVector<QPoint> &newRightLane);
    int checkLaneDeparture();
    void showNoLanePopup();

protected:
    void paintEvent(QPaintEvent *) override;

public slots:
    
};

#endif
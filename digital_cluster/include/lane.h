#ifndef LANE_H
#define LANE_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>

class Lane: public QWidget {
    Q_OBJECT

private:
    QWidget *popup;
    int res;

public:
    Lane(QWidget *parent = nullptr);
    ~Lane();
    void showNoLanePopup();
    int   get_res();
    void   set_res(int i);

protected:
    void paintEvent(QPaintEvent *) override;

    
};

#endif
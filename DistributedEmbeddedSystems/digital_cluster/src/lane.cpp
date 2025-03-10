#include "lane.h"
#include <QDir>
#include <QStyle>
#include <QColor>

Lane::Lane(QWidget *parent) : QWidget(parent)
{
    if (parent) {
        setMinimumSize(parent->width() * 0.2, parent->height() * 0.35); 
        setMaximumSize(parent->width() * 0.2, parent->height() * 0.35);
    }
    QVector<QPoint> left; 
    QVector<QPoint> right;
    int height = 500;
    for (int y = 0; y <= height; y += 50) {
        left.append(QPoint(10, y));  // Example left lane
        right.append(QPoint(80, y)); // Example right lane
    }
    updateLaneData(left, right);
}
-
void Lane::updateLaneData(const QVector<QPoint> &newLeftLane, const QVector<QPoint> &newRightLane)
{
    leftLane = newLeftLane;
    rightLane = newRightLane;
    update(); 
}

int Lane::checkLaneDeparture()
{
    int carX = width() / 2;
    int leftBoundary = leftLane.last().x();
    int rightBoundary = rightLane.last().x();
    int threshold = width() / 4; 
    if (carX - leftBoundary < threshold) {
        return 1;
    } else if (rightBoundary - carX < threshold) {
        return 2;
    }
    return 0;
}

void Lane::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw Dynamic Lane Lines
    // painter.setPen(QPen(QColor::fromHsv(0, 0, 220), 10));
    // painter.save();
    // QTransform transform;
    // transform.translate(width() / 2, height());
    // transform.scale(1, -0.3); // Adjust scale for desired perspective
    // painter.setTransform(transform);
    int i = checkLaneDeparture();

    if (i == 1)
        painter.setPen(QPen(QColor::fromHsl(60, 100, 100), 8));
    else
        painter.setPen(QPen(QColor(0, 80, 100), 8));
    // Draw left lane
    for (int i = 1; i < leftLane.size(); i++) {
        painter.drawLine(leftLane[i - 1], leftLane[i]);
    }

    // Draw right lane
    if (i == 2)
        painter.setPen(QPen(QColor::fromHsl(60, 100, 100), 8));
    else
        painter.setPen(QPen(QColor(0, 80, 100), 8));
    for (int i = 1; i < rightLane.size(); i++) {
        painter.drawLine(rightLane[i - 1], rightLane[i]);
    }

    // Draw Car
    // painter.restore();
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/car.png"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QPixmap pixmap(digital_path);  

    pixmap = pixmap.scaled(this->width() / 2, this->height() / 2, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QRect rect = QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignCenter,
        pixmap.size(),
        this->rect()
    );
    painter.drawPixmap(rect, pixmap);
}
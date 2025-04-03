#include "lane.h"
#include <QDir>
#include <QStyle>
#include <QColor>
#include <QLabel>
#include <iostream>

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
        right.append(QPoint(50, y)); // Example right lane
    }
    updateLaneData(left, right);
}

Lane::~Lane()
{
    // delete pop_up;
}

void Lane::updateLaneData(const QVector<QPoint> &newLeftLane, const QVector<QPoint> &newRightLane)
{
    leftLane = newLeftLane;
    rightLane = newRightLane;
    update(); 
}

void Lane::showNoLanePopup() {
    // pop_up = new QMessageBox(this);
    // pop_up->setText("No lanes detected, please take manual control.");
    // QString path = QCoreApplication::applicationDirPath();
    // QString digital_path = QDir(path).filePath("../fonts_icon/alert.png"); //change this dir, take out the ../ when sending to jetson
    // digital_path = QDir::cleanPath(digital_path);
    // QPixmap warningIcon(digital_path);
    // pop_up->setIconPixmap(warningIcon.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // // pop_up->setIcon(QMessageBox::Warning);
    // pop_up->setStandardButtons(QMessageBox::NoButton);
    // pop_up->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    // pop_up->setStyleSheet("background-color: rgb(0, 0, 20);"
    //     "color: white;");
    // QRect widgetRect = this->geometry();
    // int x = widgetRect.left() + (widgetRect.width() - pop_up->width()) / 2;
    // int y = widgetRect.top();  // Place at the top
    // pop_up->move(x, y);
    // QTimer::singleShot(10, pop_up, &QMessageBox::close);
    // pop_up->show();  // This does NOT block execution

    QLabel* pop_up = new QLabel(this);
    pop_up->setText("No lanes detected, please take manual control.");
    
    // Set up the icon
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/alert.png");
    digital_path = QDir::cleanPath(digital_path);
    QPixmap warningIcon(digital_path);
    pop_up->setPixmap(warningIcon.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Set up the label's appearance
    pop_up->setStyleSheet("background-color: rgb(0, 0, 20); color: white;");

    // Position the label at the top-center of the parent widget
    pop_up->setAlignment(Qt::AlignTop);

    // Make the label visible
    pop_up->show();

    // // Optionally, close the label after a certain time (e.g., 10 seconds)
    QTimer::singleShot(10000, pop_up, &QLabel::close);
}
    
    
int Lane::checkLaneDeparture()
{
    if (leftLane.empty() || rightLane.empty()) {
        return 0;
    }
    int carX = width() / 2;
    int leftBoundary = leftLane.last().x();
    int rightBoundary = rightLane.last().x();
    int threshold = width() / 4; 
    if (carX - leftBoundary < threshold) {
        return 1;
    } else if (rightBoundary - carX < threshold) {
        return 11;
    }
    return 2;
}
    
void Lane::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // Draw Car
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/theone.png"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QPixmap pixmap(digital_path);  
    int w = width();
    int h = height();
    int carX = w / 2;
    QImage img = pixmap.toImage();
    img = img.convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor color = img.pixelColor(x, y);
            if (color.alpha() == 0) 
                continue;
            if (color.red() > 100 && color.blue() < 100 && color.green() < 100)
                continue;
            
            // if (color.blue() > 100)
            // {
                int darkenedRed = color.red() * 0; 
                int darkenedGreen = color.green() * 0.4;
                int darkenedBlue = color.blue() * 0.6;
                img.setPixelColor(x, y, QColor(darkenedRed, darkenedGreen, darkenedBlue));
            // }
        }
    }
    pixmap = QPixmap::fromImage(img);
    pixmap.save("../fonts_icon/dark.png");
    // QRect carRect(carX - pixmap.width() / 2, h * 0.5, pixmap.width(), pixmap.height());
    // QPixmap scaledCar = pixmap.scaled(w / 2, h / 2, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    // painter.drawPixmap(carRect, scaledCar);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QPixmap shadowPixmap = pixmap;
    QPainter shadowPainter(&shadowPixmap);
    shadowPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    shadowPainter.fillRect(shadowPixmap.rect(), QColor(0, 0, 50, 100));  // Semi-transparent black
    shadowPainter.end();
    pixmap = pixmap.scaled(this->width() * 0.7, this->height() * 0.7, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QRect rect = QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignBottom | Qt::AlignHCenter,
        // Qt::AlignCenter,
        pixmap.size(),
        this->rect()
    );
    QRect shadowRect = rect.translated(5, 5);  // Moves shadow slightly down-right
    painter.drawPixmap(shadowRect, shadowPixmap);
    painter.drawPixmap(rect, pixmap);

    int laneBottomLeft = w * 0.1;
    int laneBottomRight = w * 0.9;
    int laneTopLeft = w * 0.3;
    int laneTopRight = w * 0.7;

    QLinearGradient laneGradient(0, h, 0, 0); 
    laneGradient.setColorAt(0.0, QColor(50, 50, 50, 150)); 
    laneGradient.setColorAt(0.4, QColor(100, 100, 100, 200)); 
    laneGradient.setColorAt(0.6, QColor(100, 100, 100, 200));
    laneGradient.setColorAt(1.0, QColor(20, 20, 20, 50)); 

    QLinearGradient redGradient(0, h, 0, 0);
    redGradient.setColorAt(0.0, QColor(50, 5, 0, 150)); 
    redGradient.setColorAt(0.4, QColor(160, 20, 0, 200)); 
    redGradient.setColorAt(0.6, QColor(160, 20, 0, 200)); 
    redGradient.setColorAt(1.0, QColor(50, 5, 0, 50)); 
    
    QColor shadowColor(30, 30, 30, 30);
    QPen shadowPen(QBrush(shadowColor), 10, Qt::SolidLine);  
    QPen right, left;
    int i = checkLaneDeparture();
    if (i == 1) {
        // QColor leftLaneColor = QColor(140, 20, 0);  
        QPen leftPen(QBrush(redGradient), 6, Qt::DashLine); 
        left = leftPen;
    }
    else {
        QPen leftPen(QBrush(laneGradient), 6, Qt::DashLine); 
        left = leftPen;
    }
    if (i == 11) {
        // QColor rightLaneColor = QColor(140, 20, 0);
        QPen rightPen(QBrush(redGradient), 6, Qt::DashLine); 
        right = rightPen;
    }
    else {
        QPen rightPen(QBrush(laneGradient), 6, Qt::DashLine);  
        right = rightPen;
    }

    painter.setPen(shadowPen);
    painter.drawLine(QPoint(laneBottomLeft - 5, h + 5), QPoint(laneTopLeft - 5, h * 0.1 + 5));
    painter.drawLine(QPoint(laneBottomRight + 5, h + 5), QPoint(laneTopRight + 5, h * 0.1 + 5));

    painter.setPen(left);
    painter.drawLine(QPoint(laneBottomLeft, h), QPoint(laneTopLeft, h * 0.1));

    painter.setPen(right);
    painter.drawLine(QPoint(laneBottomRight, h), QPoint(laneTopRight, h * 0.1));
    if (i == 0)
        showNoLanePopup();
}
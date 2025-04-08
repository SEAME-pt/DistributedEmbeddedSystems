#include "lane.h"
#include <QDir>
#include <QStyle>
#include <QColor>
#include <QLabel>
#include <iostream>
#include <QVBoxLayout>


Lane::Lane(QWidget *parent) : QWidget(parent)
{
    if (parent) {
        setMinimumSize(parent->width() * 0.2, parent->height() * 0.35); 
        setMaximumSize(parent->width() * 0.2, parent->height() * 0.35);
    }
    res = 0;
}

Lane::~Lane()
{
    // delete pop_up;
}

void Lane::showNoLanePopup() {
    // pop_up = new QMessageBox(this);
    // pop_up->setText("Please take manual control.");
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/alert.png"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
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
    // // QTimer::singleShot(10, pop_up, &QMessageBox::close);
    // pop_up->show();  // This does NOT block execution



    QWidget* popup = new QWidget(this, Qt::FramelessWindowHint | Qt::Dialog);
    popup->setAttribute(Qt::WA_DeleteOnClose);
    // popup->setStyleSheet("background-color: rgb(0, 0, 20); color: white; border: 2px solid red;");

    QHBoxLayout* layout = new QHBoxLayout(popup);
    QLabel* iconLabel = new QLabel();
    QPixmap warningIcon(digital_path);
    iconLabel->setPixmap(warningIcon.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QLabel* textLabel = new QLabel("Please take manual control.");
    textLabel->setStyleSheet("color: white; font-size: 16px;");

    layout->addWidget(iconLabel, 0, Qt::AlignBottom | Qt::AlignCenter);
    layout->addWidget(textLabel, 0, Qt::AlignCenter);

    popup->setLayout(layout);
    popup->adjustSize();

    // Center horizontally, top aligned
    QRect widgetRect = this->geometry();
    int x = widgetRect.left() + (widgetRect.width() - popup->width()) / 2;
    int y = widgetRect.top();
    popup->move(x, y);


    popup->show();
    // QTimer::singleShot(2000, popup, &QWidget::close);

    // QLabel* pop_up = new QLabel(this);
    // pop_up->setText("No lanes detected, please take manual control.");
    // QString path = QCoreApplication::applicationDirPath();
    // QString digital_path = QDir(path).filePath("../fonts_icon/alert.png");
    // digital_path = QDir::cleanPath(digital_path);
    // QPixmap warningIcon(digital_path);
    // pop_up->setPixmap(warningIcon.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // pop_up->setStyleSheet("background-color: rgb(0, 0, 20); color: white;");
    // pop_up->setAlignment(Qt::AlignTop);
    // pop_up->show();
    // QTimer::singleShot(10000, pop_up, &QLabel::close);
}
    
void Lane::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // Draw Car
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/dark2.png"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QPixmap pixmap(digital_path);  
    int w = width();
    int h = height();
    int carX = w / 2;
    // QImage img = pixmap.toImage();
    // img = img.convertToFormat(QImage::Format_ARGB32);
    // for (int y = 0; y < img.height(); ++y) {
    //     for (int x = 0; x < img.width(); ++x) {
    //         QColor color = img.pixelColor(x, y);
    //         if (color.alpha() == 0) 
    //             continue;
    //         if (color.red() > 100 && color.blue() < 100 && color.green() < 100)
    //             continue;
    //         if (color.red() < 50 && color.blue() < 50 && color.green() < 50)
    //             continue;
            
    //         // int brightness = qMax(qMax(color.red(), color.green()), color.blue());

    //         // // Only dim very bright pixels
    //         // if (brightness > 200) {
    //             int red   = color.red()   * 0.2;
    //             int green = color.green() * 0.4;
    //             int blue  = color.blue()  * 0.5;
    //             img.setPixelColor(x, y, QColor(red, green, blue, color.alpha()));
    //         // }
    //     }
    // }
    // pixmap = QPixmap::fromImage(img);
    // pixmap.save("../fonts_icon/dark3.png");
    QRect carRect(carX - pixmap.width() / 2, h * 0.5, pixmap.width(), pixmap.height());
    QPixmap scaledCar = pixmap.scaled(w / 2, h / 2, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    painter.drawPixmap(carRect, scaledCar);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QPixmap shadowPixmap = pixmap;
    QPainter shadowPainter(&shadowPixmap);
    shadowPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    shadowPainter.fillRect(shadowPixmap.rect(), QColor(0, 0, 20, 200));  // Semi-transparent black
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
    laneGradient.setColorAt(0.0, QColor(20, 20, 20, 50)); 
    laneGradient.setColorAt(0.4, QColor(100, 100, 100, 175)); 
    laneGradient.setColorAt(0.6, QColor(100, 100, 100, 175));
    laneGradient.setColorAt(1.0, QColor(20, 20, 20, 50)); 

    QLinearGradient redGradient(0, h, 0, 0);
    redGradient.setColorAt(0.0, QColor(50, 5, 0, 50)); 
    redGradient.setColorAt(0.4, QColor(160, 20, 0, 175)); 
    redGradient.setColorAt(0.6, QColor(160, 20, 0, 175)); 
    redGradient.setColorAt(1.0, QColor(50, 5, 0, 50)); 
    
    QColor shadowColor(30, 30, 30, 30);
    QPen shadowPen(QBrush(shadowColor), 10, Qt::SolidLine);  
    QPen right, left;
    if (res == 76) {
        QPen leftPen(QBrush(redGradient), 5, Qt::DashLine); 
        left = leftPen;
    }
    else {
        QPen leftPen(QBrush(laneGradient), 5, Qt::DashLine); 
        left = leftPen;
    }
    if (res == 82) {
        QPen rightPen(QBrush(redGradient), 5, Qt::DashLine); 
        right = rightPen;
    }
    else {
        QPen rightPen(QBrush(laneGradient), 5, Qt::DashLine);  
        right = rightPen;
    }
    if (res == 0) {
        QPen leftPen(QBrush(redGradient), 5, Qt::DashLine); 
        left = leftPen;
        QPen rightPen(QBrush(redGradient), 5, Qt::DashLine); 
        right = rightPen;
        showNoLanePopup();
    }
    painter.setPen(shadowPen);
    painter.drawLine(QPoint(laneBottomLeft - 5, h + 5), QPoint(laneTopLeft - 5, h * 0.1 + 5));
    painter.drawLine(QPoint(laneBottomRight + 5, h + 5), QPoint(laneTopRight + 5, h * 0.1 + 5));
    painter.setPen(left);
    painter.drawLine(QPoint(laneBottomLeft, h), QPoint(laneTopLeft, h * 0.1));
    painter.setPen(right);
    painter.drawLine(QPoint(laneBottomRight, h), QPoint(laneTopRight, h * 0.1));
}

void   Lane::set_res(int i)
{
    res = i;
}

int   Lane::get_res()
{
    return res;
}
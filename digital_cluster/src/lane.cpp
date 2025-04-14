#include "lane.h"

Lane::Lane(QWidget *parent) : QWidget(parent), res(0)
{
    if (parent) {
        setMinimumSize(parent->width() * 0.15, parent->height() * 0.25); 
        setMaximumSize(parent->width() * 0.15, parent->height() * 0.25);
    }
}

Lane::~Lane()
{
}

void Lane::showNoLanePopup()
{
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/alert.png"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);

    popup = new QWidget(this, Qt::FramelessWindowHint | Qt::Dialog);
    popup->setAttribute(Qt::WA_DeleteOnClose);

    QHBoxLayout* layout = new QHBoxLayout(popup);
    QLabel* iconLabel = new QLabel(popup);
    QPixmap warningIcon(digital_path);
    iconLabel->setPixmap(warningIcon.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QLabel* textLabel = new QLabel(" Please take manual control.", popup);
    textLabel->setStyleSheet("color: white; font-size: 16px;");

    layout->addWidget(iconLabel, 0, Qt::AlignTop | Qt::AlignCenter);
    layout->addWidget(textLabel, 0, Qt::AlignCenter);

    popup->setLayout(layout);
    popup->adjustSize();

    QRect widgetRect = this->geometry();
    int x = widgetRect.left() + (widgetRect.width() - popup->width()) / 2;
    int y = widgetRect.top();
    popup->move(x, y);
    popup->show();
}

/**
 * @brief 
 * @
 * @param event 
 */
void Lane::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/dark5.png"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QPixmap pixmap(digital_path);  
    int w = width();
    int h = height();
    int carX = w / 2;
    pixmap = pixmap.scaled(this->width() * 0.7, this->height() * 0.7, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QRect rect = QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignBottom | Qt::AlignHCenter,
        pixmap.size(),
        this->rect()
    );
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(rect, pixmap);
    QPixmap shadowPixmap = pixmap;
    QPainter shadowPainter(&shadowPixmap);
    shadowPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    shadowPainter.fillRect(shadowPixmap.rect(), QColor(0, 0, 20, 200));  // Semi-transparent black
    shadowPainter.end();
    // QRect shadowRect = rect.translated(5, 5);  // Moves shadow slightly down-right
    // painter.drawPixmap(shadowRect, shadowPixmap);

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
    QPen left(QBrush(laneGradient), 5, Qt::DashLine); 
    QPen right(QBrush(laneGradient), 5, Qt::DashLine);  
    if (res == 0) {
        // QPen leftPen(QBrush(redGradient), 5, Qt::DashLine); 
        // left = leftPen;
        // QPen rightPen(QBrush(redGradient), 5, Qt::DashLine); 
        // right = rightPen;
        showNoLanePopup();
    }
    else {
        if (res == 76) {
            QPen leftPen(QBrush(redGradient), 5, Qt::DashLine); 
            left = leftPen;
        }
        else if (res == 82) {
            QPen rightPen(QBrush(redGradient), 5, Qt::DashLine); 
            right = rightPen;
        }
        if (popup) {
            popup->close();
            popup->deleteLater();
            popup = nullptr;
        }
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
    update();
}

int   Lane::get_res()
{
    return res;
}
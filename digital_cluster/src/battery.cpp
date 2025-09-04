#include "../include/battery.h"

Battery::Battery(QWidget *parent)
    : QWidget(parent), current(20), max(100), test_painter(nullptr)
{
    if (parent)
    {
        setMinimumSize(parent->width() * 0.5, parent->height() * 0.7); 
        // setMaximumSize(parent->width() * 0.5, parent->height() * 0.7);
    }
}

Battery::~Battery()
{
    std::cout << "Remove Battery" << std::endl;
}

void Battery::set_current(int n)
{
    current = n;
    update();
}

int Battery::get_current()
{
    return current;
}

void Battery::paintEvent(QPaintEvent *event)
{
    TestPainter* painter = test_painter;
    QPainter real_painter(this);
    if (!painter)
    {
        painter = new QPainterCaller(&real_painter);
        flag = 1;
    }
    if (test_painter)
    {
        painter->begin(this);
    }
    painter->setRenderHint(QPainter::Antialiasing, true);
    draw_arcs(painter);
    draw_pixmap(painter);
    if (flag == 1 && painter)
    {
        delete painter;  // Clean up if we allocated it
    }
}


//fading arcs
void Battery::draw_arcs(TestPainter *painter)
{
    int radius = qMin(width(), height()) / 2.5;  
    int segments = 300; 
    int centerX = width() / 2;
    int centerY = height() / 2;
    float segment_angle = 270.0f / segments; 
    for (int i = 0; i < segments; ++i)
    {
        float t = static_cast<float>(i) / (segments);
        int alpha = static_cast<int>(120 * (1 - std::abs(2 * t - 1))); 
        QColor color(0, 52, 50, alpha); 
        QPen pen(color, width() / 50);
        painter->setPen(pen);
        float overlap = 1.1f; 
        painter->drawArc(centerX - radius, centerY - radius, radius * 2, radius * 2, (180 - i * segment_angle) * 16, -segment_angle * 16 * overlap);
    }
    float angle_progress = (static_cast<float>(current) * 270.0f) / max;
    segment_angle = angle_progress / segments;
    QColor start_color(0, 80, 60); 
    QColor end_color(0, 255, 200);
    if (current < 40)
    {
        segments = 200;
        start_color = QColor(0, 80, 60); 
        end_color = QColor(0, 100, 150, 255); 
    }
    for (int i = 0; i < segments; ++i)
    {
        float t = static_cast<float>(i) / segments; //factor (0 to 1)
        QColor color = QColor::fromRgbF(
            (1 - t) * start_color.redF() + t * end_color.redF(),  
            (1 - t) * start_color.greenF() + t * end_color.greenF(), 
            (1 - t) * start_color.blueF() + t * end_color.blueF()  
        );
        int alpha = static_cast<int>(255 * (1 - std::abs(2 * t - 1)));
        color.setAlpha(alpha);
        QPen pen(color, width() / 50);
        painter->setPen(pen);
        painter->drawArc(centerX - radius, centerY - radius, radius * 2, radius * 2, (270 + i * segment_angle) * 16, segment_angle * 16);
    }
}

void Battery::draw_pixmap(TestPainter *painter)
{
    painter->setPen(QPen(QColor(0, 255, 190)));
    painter->setFont(QFont("Digital-7", width() / 5, QFont::Bold));
    QRect currentTextRect = painter->boundingRect(rect(), Qt::AlignCenter, QString::number(current));
    painter->drawText(currentTextRect, Qt::AlignCenter, QString::number(current));
    
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/battery.png");
    digital_path = QDir::cleanPath(digital_path);
    QPixmap pixmap(digital_path);  
    pixmap = pixmap.scaled(width() / 8, width() / 8, Qt::KeepAspectRatio);
    QRect rectBottom = this->rect();
    int xIcon = (width() - pixmap.width()) / 2;  
    QRect bottomRect = QRect(xIcon, currentTextRect.bottom() + 10, pixmap.width(), pixmap.height());
    painter->drawPixmap(bottomRect, pixmap);
    draw_text(painter, bottomRect);
}

void Battery::draw_text(TestPainter *painter, QRect bottomRect)
{
    QFont font("Calculator", width() / 12);
    painter->setFont(font);
    painter->setPen(QPen(QColor(0, 120, 100)));
    QRectF textRect(bottomRect.right() + 5, bottomRect.bottom() - 23, 30, 30);  // Adjust size as needed
    int flags = Qt::AlignLeft | Qt::AlignVCenter;  // Adjust alignment flags as needed
    QString text = "%";
    painter->drawText(textRect, flags, text);
}


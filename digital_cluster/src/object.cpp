#include "object.h"

Object::Object(QWidget *parent)
{
    // QString path = QCoreApplication::applicationDirPath();
    // digital_path = QDir(path).filePath("../fonts_icon/warning1.png");
    // digital_path = QDir::cleanPath(digital_path);
    set_object(1);
}

// void Object::showObjectPopup()
// {
//     if (!popup && object > 3)
//     {
//         popup = new QWidget(this);
//         popup->setAttribute(Qt::WA_DeleteOnClose);
//         popup->setStyleSheet("background-color: transparent;");
//         QHBoxLayout* layout = new QHBoxLayout(popup);

//         QLabel* textLabel = new QLabel("Colision danger", popup);
//         textLabel->setStyleSheet("color: gray; font-size: 18px;");
//         layout->addWidget(textLabel, 0, Qt::AlignCenter);
//         popup->setLayout(layout);
//         popup->adjustSize();
//         popup->move((width() - popup->width()) / 2, 0);

//         opacityEffect = new QGraphicsOpacityEffect(popup);
//         opacityEffect->setOpacity(0.0);
//         popup->setGraphicsEffect(opacityEffect);
//         popup->show();

//         QPropertyAnimation* fadeIn = new QPropertyAnimation(opacityEffect,
//         "opacity"); fadeIn->setDuration(300); fadeIn->setStartValue(0.0);
//         fadeIn->setEndValue(1.0);
//         fadeIn->setEasingCurve(QEasingCurve::InOutSine);
//         fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
//     }
// if (lane == 0 || object > 3)
// {
//     QPixmap warningIcon(digital_path);
//     int size = 35;
//     int x = (width() - size) / 2;
//     int y = (height() - size) / 2;

//     painter.drawPixmap(x, y, warningIcon.scaled(size, size,
//     Qt::KeepAspectRatio, Qt::SmoothTransformation));
// }
// }

// void Object::setting_pens(int i, QPainter *painter)
// {
//     QColor rectColor = (objects[i] > 10) ? Qt::darkRed : Qt::darkGray;
//     // std::cout << "Object " << i << ": " << objects[i] << std::endl;
//     // std::cout << "Color: " << rectColor.red() << std::endl;
//     QPen rectPen(rectColor, 5);
//     QBrush rectBrush(rectColor, Qt::SolidPattern);
//     painter->setPen(rectPen);
//     painter->setBrush(rectBrush);
//     // painter.setOpacity(1.0);
// }

void Object::paintEvent(QPaintEvent *event)
{
//     Lane::paintEvent(event);
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing);
//     int BottomLeft = width() * 0.35;
//     int BottomRight = width() * 0.65;
//     int TopLeft = width() * 0.4;
//     int TopRight = width() * 0.6;

//     int rectWidth = 9;
//     int margin = 10;
//     int centerX = (width() - rectWidth) / 2;

//     if (objects[0] != 0)
//     {
//         setting_pens(0, &painter);
//         painter.drawLine(QPoint(BottomLeft, height() * 0.15), QPoint(TopLeft - margin, height() * 0.1));
//     }
//     if (objects[1] != 0)
//     {
//         setting_pens(1, &painter);
//         painter.drawLine(QPoint(centerX, height() * 0.05), QPoint((width() + rectWidth) / 2, height() * 0.05));
//     }
//     if (objects[2] != 0)
//     {
//         setting_pens(2, &painter);
//         painter.drawLine(QPoint(BottomRight, height() * 0.15), QPoint(TopRight + margin, height() * 0.1));
//     }
}

//parsing
void Object::set_object(int i)
{
    if (object == i or (i != 1 && i != 2))
        return;
    object = i;
    std::cout << "Object: " << object << std::endl;
    // objects.clear();
    // objects.resize(3, 0);
    // objects[0] = i % 10; // left
    // objects[1] = (i / 10) % 10; // center
    // objects[2] = (i / 100) % 10; // right
    // std::cout << "Object: " << objects[0] << " " << objects[1] << " " << objects[2] << std::endl;
    // update();
}

//     // std::vector<int> temp(3, 0);
//     // int j = -1;
//     // for (int i = 0; i < str.size(); i++)
//     // {
//     //     if (str[i] == ',' || i == str.size() - 1)
//     //     {
//     //         int n = i;
//     //         while (n - 1 >= 0 && str[n - 1] != ',')
//     //             n--;
//     //         if (i == str.size() - 1)
//     //             i++;
//     //         temp[++j] = std::stoi(str.substr(n, i - n));
//     //     }
//     // }

//     // if (objects == temp)
//     //     return;
//     // objects = temp;
//     // std::cout << "Object: " << objects[0] << " " << objects[1] << " " << objects[2] << std::endl;
//     update();


int Object::get_object()
{
    return object;
}

Object::~Object()
{
    // if (popup)
    // {
    //     popup->close();
    //     popup->deleteLater();
    //     popup = nullptr;
    // }
    std::cout << "Remove Object" << std::endl;
}
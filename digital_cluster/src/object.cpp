#include "object.h"

Object::Object(QWidget *parent): QWidget(parent)
{
    QString path = QCoreApplication::applicationDirPath();
    speed50_path = QDir(path).filePath("../fonts_icon/speed50.png");
    speed50_path = QDir::cleanPath(speed50_path);
    speed80_path = QDir(path).filePath("../fonts_icon/speed80.png");
    speed80_path = QDir::cleanPath(speed80_path);
    set_object(2);
}

void Object::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPixmap pixmap;
    if (object == 1) {
        pixmap.load(speed50_path);
    } else if (object == 2) {
        pixmap.load(speed80_path);
    }
    if (!pixmap.isNull()) {
        QPixmap scaled = pixmap.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(0, 0, scaled);  //  margin
    }
    else {
        std::cout << "Object: no image found" << std::endl;
    }
}

//parsing
void Object::set_object(int i)
{
    if (object == i or (i != 1 && i != 2))
        return;
    object = i;
    update();
}

int Object::get_object()
{
    return object;
}

Object::~Object()
{
    std::cout << "Remove Object" << std::endl;
}
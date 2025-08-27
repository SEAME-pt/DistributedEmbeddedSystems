#include "object.h"

Object::Object(QWidget *parent): QWidget(parent)
{
    QString path = QCoreApplication::applicationDirPath();
    speed50_path = QDir(path).filePath("../fonts_icon/speed50.png");
    speed50_path = QDir::cleanPath(speed50_path);
    speed80_path = QDir(path).filePath("../fonts_icon/speed80.png");
    speed80_path = QDir::cleanPath(speed80_path);
    object = 0;
    // set_object(1, "jetracer/speed_50");
}

void Object::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPixmap pixmap;
    if (object == 11) {
        pixmap.load(speed50_path);
    } else if (object == 1) {
        pixmap.load(speed80_path);
    }
    if (!pixmap.isNull()) {
        QPixmap scaled = pixmap.scaled(55, 55, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(0, 0, scaled);  //  margin
    }
    else {
        std::cout << "Object: no image found" << std::endl;
    }
}

//parsing
void Object::set_object(int i, const QString &topicName)
{
    if (object == i or (i != 1 && i != 0))
        return;
    if (topicName == "jetracer/speed_50") {
        object = i + 10;
    } else {
        object = i;
    }
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
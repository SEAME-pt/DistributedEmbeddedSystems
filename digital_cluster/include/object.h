#ifndef Object_H
#define Object_H

#include "lane.h"
#include <string>
#include <vector>

class Object : public Lane {
  Q_OBJECT

private:
  QWidget *popup = nullptr;
  std::vector<int> objects;
  QString digital_path;

public:
  Object(QWidget *parent = nullptr);
  ~Object();
  // void showObjectPopup();
  void setting_pens(int i, QPainter *painter);
  std::vector<int> get_object();
  void set_object(std::string str);

protected:
  void paintEvent(QPaintEvent *) override;

signals:
};

#endif
#ifndef Object_H
#define Object_H

#include "lane.h"
#include <string>
#include <vector>

class Object : public QWidget {
  Q_OBJECT

private:
  QWidget *popup = nullptr;
  int object;
  QString speed50_path;
  QString speed80_path;

public:
  Object(QWidget *parent = nullptr);
  ~Object();
  int get_object();
  QString get_speed50_path() const { return speed50_path; }
  QString get_speed80_path() const { return speed80_path; }
  void set_object(int i, const QString &topicName);

protected:
  void paintEvent(QPaintEvent *) override;

signals:
};

#endif
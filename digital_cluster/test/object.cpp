#include "../include/mainwindow.h"
#include <QDebug>
#include <QTest>
#include <gtest/gtest.h>

class ObjectT : public testing::Test {
protected:
  MainWindow *mw;
  Object *object;

  void SetUp() override {
    mw = new MainWindow();
    mw->show();
    object = mw->get_object();
  }
  void TearDown() override { delete mw; }
};

TEST_F(ObjectT, TestVar) {
  object->set_object(1, "jetracer/speed_50");
  EXPECT_EQ(object->get_object(), 11);

  QPixmap pixmap;
  QString path = object->get_speed50_path();
  QVERIFY(pixmap.load(path));

  path = object->get_speed80_path();
  QVERIFY(pixmap.load(path));

  object->set_object(1, "jetracer/speed_80");
  EXPECT_EQ(object->get_object(), 1);

  object->set_object(2, "jetracer/speed_80");
  EXPECT_EQ(object->get_object(), 1);
}

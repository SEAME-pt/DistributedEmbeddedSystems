#include "../include/mainwindow.h"
#include <QDebug>
#include <QSignalSpy>
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
  object->set_object(1);
  QString path = object->get_speed50_path();

  QPixmap pixmap;
  QVERIFY(pixmap.load(path));
  path = object->get_speed80_path();
  QVERIFY(pixmap.load(path));

  EXPECT_EQ(object->get_object(), 1);

  object->set_object(3);
  EXPECT_EQ(object->get_object(), 1);
}

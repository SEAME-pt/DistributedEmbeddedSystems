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
    object = mw->get_lane();
  }
  void TearDown() override { delete mw; }
};

// TEST_F(ObjectT, TestVar) {
//   object->set_object("0);
//   QTest::qWait(450);
//   EXPECT_EQ(object->get_object(), 0);
// }
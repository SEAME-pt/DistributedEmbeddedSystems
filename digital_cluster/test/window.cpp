#include <gtest/gtest.h>
#include <QApplication>
#include <QMetaObject>
#include <QDebug>
#include <QTest>
#include "../include/mainwindow.h"

class Window : public testing::Test {
protected:
    MainWindow* window;
    void SetUp() override {  // new before each test
        window = new MainWindow();
    }
    void TearDown() override { //delete after each test
        delete window;
    }
};

TEST_F(Window, Mqtt) {
    QTest::qWait(1000);
    EXPECT_EQ(window->get_client()->state(), QMqttClient::Connected);
    //check signal connection 
    const QMetaObject *meta = window->get_client()->metaObject();
    EXPECT_TRUE(meta->indexOfSignal("connected()") >= 0);
    EXPECT_TRUE(meta->indexOfSignal(QMetaObject::normalizedSignature("messageReceived(QByteArray,QMqttTopicName)")) >= 0);
}
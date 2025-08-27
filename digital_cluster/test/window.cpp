#include <gtest/gtest.h>
#include <QApplication>
#include <QMetaObject>
#include <QDebug>
#include <QSignalSpy>
#include <QTest>
#include "../include/mainwindow.h"

class Window : public testing::Test
{
    protected:
        MainWindow* window;

    void SetUp() override {  // new before each test
        window = new MainWindow;
    }
    void TearDown() override { //delete after each test
        delete window;
    }
};

TEST_F(Window, Mqtt)
{
    const QMetaObject *meta = window->get_client()->metaObject();
    EXPECT_TRUE(meta->indexOfSignal("connected()") >= 0);
    EXPECT_TRUE(meta->indexOfSignal(QMetaObject::normalizedSignature("messageReceived(QByteArray,QMqttTopicName)")) >= 0);

    window->get_client()->messageReceived("5", QMqttTopicName("jetracer/speed"));
    window->get_client()->messageReceived("1", QMqttTopicName("jetracer/speed_50"));
    window->get_client()->messageReceived("80", QMqttTopicName("jetracer/battery"));
    window->get_client()->messageReceived("35", QMqttTopicName("jetracer/temperature"));
    window->get_client()->messageReceived("8", QMqttTopicName("jetracer/autonomy"));
    window->get_client()->messageReceived("82", QMqttTopicName("jetracer/lane_touch"));
    
    EXPECT_EQ(window->get_speed()->get_current(), static_cast<int>(5 * 3.6));
    EXPECT_EQ(window->get_battery()->get_current(), 80);
    EXPECT_EQ(window->get_temperature()->get_temperature(), 35);
    EXPECT_EQ(window->get_autonomy()->get_autonomy(), 8);
    EXPECT_EQ(window->get_lane()->get_lane(), 82);
    EXPECT_EQ(window->get_object()->get_object(), 11); // speed_50
    
    window->get_client()->messageReceived("1", QMqttTopicName("jetracer/speed_80"));
    EXPECT_EQ(window->get_object()->get_object(), 1); // speed_80

}
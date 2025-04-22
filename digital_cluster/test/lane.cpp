#include <gtest/gtest.h>
#include <QDebug>
#include <QTest>
#include <QSignalSpy>
#include "../include/mainwindow.h"

class LaneT : public testing::Test
{
    protected:
        MainWindow* mw;
        Lane* lane;

    void SetUp() override
    {
        mw = new MainWindow();
        mw->show();
        lane = mw->get_lane();
    }
    void TearDown() override
    {
        delete mw;
    }
};

TEST_F(LaneT, TestVar)
{
    lane->set_lane(0);
    EXPECT_EQ(lane->get_lane(), 0);

    lane->set_lane(82);
    EXPECT_EQ(lane->get_lane(), 82);
    
    lane->set_lane(76);
    EXPECT_EQ(lane->get_lane(), 76);

    QSignalSpy spy(&lane, &Lane::leftDashOffsetChanged);
    lane.leftAnimation->start();
    QTest::qWait(2000);
    EXPECT_GT(spy.count(), 0);

    // EXPECT_TRUE(lane->get_layout() != nullptr);
    // EXPECT_TRUE(lane->get_mainlayout() != nullptr);
    // EXPECT_TRUE(lane->get_label() != nullptr);
}
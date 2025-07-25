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
    QTest::qWait(500);
    EXPECT_EQ(lane->get_lane(), 0);
    EXPECT_NE(lane->get_popup(), nullptr);
    EXPECT_NEAR(lane->leftOpacity(), 0.0, 0.01);
    EXPECT_NEAR(lane->rightOpacity(), 0.0, 0.01);

    lane->set_lane(82);
    QTest::qWait(500);
    EXPECT_EQ(lane->leftOpacity(), 0.0);
    EXPECT_NEAR(lane->rightOpacity(), 1.0, 0.01);
    
    lane->set_lane(76);
    QTest::qWait(450);
    EXPECT_NEAR(lane->leftOpacity(), 1.0, 0.01);
    EXPECT_NEAR(lane->rightOpacity(), 0.0, 0.01);

    lane->set_lane(2);
    QTest::qWait(450);
    EXPECT_NEAR(lane->leftOpacity(), 0.0, 0.01);
    EXPECT_NEAR(lane->rightOpacity(), 0.0, 0.01);
}

TEST_F(LaneT, SetLaneEdgeCases) {
    lane->set_lane(-1); 
    QTest::qWait(500);
    EXPECT_NEAR(lane->leftOpacity(), 0.0, 0.01);
    EXPECT_NEAR(lane->rightOpacity(), 0.0, 0.01);

    lane->set_lane(std::numeric_limits<int>::max()); // Large value
    QTest::qWait(500);
    EXPECT_NEAR(lane->leftOpacity(), 0.0, 0.01); 
    EXPECT_NEAR(lane->rightOpacity(), 0.0, 0.01);
}

TEST_F(LaneT, DashOffsetAnimation) {
    QTest::qWait(80);
    EXPECT_EQ(lane->leftDashOffset(), 1.0); // After int(... + 1) % 5
    EXPECT_EQ(lane->rightDashOffset(), 1.0);
}

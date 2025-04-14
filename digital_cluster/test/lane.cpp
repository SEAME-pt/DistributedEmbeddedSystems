#include <gtest/gtest.h>
#include <QDebug>
#include "../include/mainwindow.h"

class LaneT : public testing::Test
{
protected:
    MainWindow* mw;
    Lane* Lane;
    void SetUp() override
    {
        mw = new MainWindow();
        mw->show();
        Lane = mw->get_lane();
    }
    void TearDown() override
    {
        delete mw;
    }
};

TEST_F(LaneT, TestVar)
{
    Lane->set_res(0);
    EXPECT_EQ(Lane->get_res(), 0);
    Lane->set_res(82);
    EXPECT_EQ(Lane->get_res(), 82);
    Lane->set_res(76);
    EXPECT_EQ(Lane->get_res(), 76);
    // EXPECT_TRUE(Lane->get_layout() != nullptr);
    // EXPECT_TRUE(Lane->get_mainlayout() != nullptr);
    // EXPECT_TRUE(Lane->get_label() != nullptr);
}
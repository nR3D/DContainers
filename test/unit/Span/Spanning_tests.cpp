#include <gtest/gtest.h>
#include "DContainers/Span/Spanning.hpp"

using mdc::Spanning;

class SpanTest : public ::testing::Test {
protected:
    Spanning all = Spanning::all();
    Spanning interval = Spanning(3, 7);
    Spanning index = Spanning(5);
};

TEST_F(SpanTest, AllCheck) {
    EXPECT_TRUE(all.isAll);
}

TEST_F(SpanTest, FromToCheck) {
    EXPECT_EQ(interval.from, 3);
    EXPECT_EQ(interval.to, 7);
    EXPECT_FALSE(interval.isAll);
}

TEST_F(SpanTest, IndexCheck) {
    EXPECT_EQ(index.from, 5);
    EXPECT_EQ(index.to, 5);
    EXPECT_FALSE(index.isAll);
}

TEST_F(SpanTest, EqualityCheck) {
    EXPECT_EQ(all, Spanning::all());
    EXPECT_EQ(interval, Spanning(3, 7));
    EXPECT_EQ(index, Spanning(5));
    EXPECT_EQ(index, Spanning(5, 5));
}

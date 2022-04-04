#include <gtest/gtest.h>
#include "../../src/Span/Span.hpp"

class SpanTest : public ::testing::Test {
protected:
    Span all = Span::all();
    Span interval = Span(3,7);
    Span index = Span(5);
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
    EXPECT_EQ(all, Span::all());
    EXPECT_EQ(interval, Span(3,7));
    EXPECT_EQ(index, Span(5));
    EXPECT_EQ(index, Span(5,5));
}

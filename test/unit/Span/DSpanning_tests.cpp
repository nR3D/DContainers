#include <gtest/gtest.h>
#include "DContainers/Span/DSpanning.hpp"

using mdc::DSpanning, mdc::SpanSize, mdc::Spanning;

class DSpanTest : public ::testing::Test {
protected:
    DSpanning<SpanSize::All> all = DSpanning<SpanSize::All>{};
    DSpanning<SpanSize::Interval<3,7>> interval = DSpanning<SpanSize::Interval<3,7>>{};
    DSpanning<SpanSize::Interval<5>> intervalSize = DSpanning<SpanSize::Interval<5>>{3, 7};
    DSpanning<SpanSize::Index<5>> index = DSpanning<SpanSize::Index<5>>{};
};

TEST_F(DSpanTest, AllCheck) {
    EXPECT_TRUE(all.isAll);
}

TEST_F(DSpanTest, FromToCheck) {
    EXPECT_EQ(interval.from, 3);
    EXPECT_EQ(interval.to, 7);
    EXPECT_FALSE(interval.isAll);

    EXPECT_EQ(intervalSize.from, 3);
    EXPECT_EQ(intervalSize.to, 7);
    EXPECT_FALSE(intervalSize.isAll);
}

TEST_F(DSpanTest, IndexCheck) {
    EXPECT_EQ(index.from, 5);
    EXPECT_EQ(index.to, 5);
    EXPECT_FALSE(index.isAll);
}

TEST_F(DSpanTest, EqualityCheck) {
    EXPECT_EQ(all, Spanning::all());
    EXPECT_EQ(all, DSpanning<SpanSize::All>{});

    EXPECT_EQ(interval, Spanning(3, 7));
    auto cmpInterval = DSpanning<SpanSize::Interval<3,7>>{};
    EXPECT_EQ(interval, cmpInterval);
    EXPECT_EQ(interval, intervalSize);
    auto cmpIntervalSize = DSpanning<SpanSize::Interval<5>>{3, 7};
    EXPECT_EQ(intervalSize, cmpIntervalSize);

    EXPECT_EQ(index, Spanning(5));
    EXPECT_EQ(index, DSpanning<SpanSize::Index<5>>{});
    auto cmpIndex = DSpanning<SpanSize::Interval<5,5>>{};
    EXPECT_EQ(index, cmpIndex);
}

TEST_F(DSpanTest, SizeMismatch_ThrowLengthError) {
    EXPECT_THROW(DSpanning<SpanSize::Interval<4>>(3, 7), std::length_error);
}

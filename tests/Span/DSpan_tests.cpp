#include <gtest/gtest.h>
#include "../../src/Span/DSpan.hpp"

class DSpanTest : public ::testing::Test {
protected:
    DSpan<SpanSize::All> all = DSpan<SpanSize::All>{};
    DSpan<SpanSize::Interval<3,7>> interval = DSpan<SpanSize::Interval<3,7>>{};
    DSpan<SpanSize::Interval<5>> intervalSize = DSpan<SpanSize::Interval<5>>{3,7};
    DSpan<SpanSize::Index<5>> index = DSpan<SpanSize::Index<5>>{};
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
    EXPECT_EQ(all, Span::all());
    EXPECT_EQ(all, DSpan<SpanSize::All>{});

    EXPECT_EQ(interval, Span(3,7));
    auto cmpInterval = DSpan<SpanSize::Interval<3,7>>{};
    EXPECT_EQ(interval, cmpInterval);
    EXPECT_EQ(interval, intervalSize);
    auto cmpIntervalSize = DSpan<SpanSize::Interval<5>>{3,7};
    EXPECT_EQ(intervalSize, cmpIntervalSize);

    EXPECT_EQ(index, Span(5));
    EXPECT_EQ(index, DSpan<SpanSize::Index<5>>{});
    auto cmpIndex = DSpan<SpanSize::Interval<5,5>>{};
    EXPECT_EQ(index, cmpIndex);
}

TEST_F(DSpanTest, SizeMismatch_ThrowLengthError) {
    EXPECT_THROW(DSpan<SpanSize::Interval<4>>(3,7), std::length_error);
}

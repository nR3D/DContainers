#include <gtest/gtest.h>
#include "DContainers/Span.hpp"

using mdc::Spanning, mdc::DSpanning, mdc::Span, mdc::SpanSize;

TEST(SpannedTest, EqualityAllCheck) {
    EXPECT_EQ(Span::all(), Spanning::all());
    EXPECT_EQ(Span::all(), DSpanning<SpanSize::All>{});
}

TEST(SpannedTest, EqualityIntervalCheck) {
    EXPECT_EQ(Span::of(3, 7), Spanning(3, 7));

    auto wrapInterval = Span::of<3,7>(),
            cmpInterval = DSpanning<SpanSize::Interval<3,7>>();
    EXPECT_EQ(wrapInterval, cmpInterval);

    auto wrapIntervalSize = Span::of<5>(3, 7);
    EXPECT_EQ(wrapIntervalSize, DSpanning<SpanSize::Interval<5>>(3, 7));

    EXPECT_EQ(wrapInterval, wrapIntervalSize);
    EXPECT_EQ(wrapInterval, Span::of(3, 7));
}

TEST(SpannedTest, EqualityIndexCheck) {
    EXPECT_EQ(Span::of(5), Spanning(5));
    EXPECT_EQ(Span::of<5>(), DSpanning<SpanSize::Index<5>>{});
    EXPECT_EQ(Span::of(5), Span::of<5>());
}

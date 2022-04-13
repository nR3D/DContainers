#include <gtest/gtest.h>
#include "DContainers/Span/SpanWrapper.hpp"

using mdc::Span, mdc::DSpan, mdc::SpanWrapper, mdc::SpanSize;

TEST(SpanWrapperTest, EqualityAllCheck) {
    EXPECT_EQ(SpanWrapper::all(), Span::all());
    EXPECT_EQ(SpanWrapper::all(), DSpan<SpanSize::All>{});
}

TEST(SpanWrapperTest, EqualityIntervalCheck) {
    EXPECT_EQ(SpanWrapper::interval(3,7), Span(3,7));

    auto wrapInterval = SpanWrapper::interval<3,7>(),
            cmpInterval = DSpan<SpanSize::Interval<3,7>>();
    EXPECT_EQ(wrapInterval, cmpInterval);

    auto wrapIntervalSize = SpanWrapper::interval<5>(3,7);
    EXPECT_EQ(wrapIntervalSize, DSpan<SpanSize::Interval<5>>(3,7));

    EXPECT_EQ(wrapInterval, wrapIntervalSize);
    EXPECT_EQ(wrapInterval, SpanWrapper::interval(3,7));
}

TEST(SpanWrapperTest, EqualityIndexCheck) {
    EXPECT_EQ(SpanWrapper::index(5), Span(5));
    EXPECT_EQ(SpanWrapper::index<5>(), DSpan<SpanSize::Index<5>>{});
    EXPECT_EQ(SpanWrapper::index(5), SpanWrapper::index<5>());
}

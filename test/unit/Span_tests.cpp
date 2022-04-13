/*
 *   Copyright 2022 Alberto Guarnieri
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

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

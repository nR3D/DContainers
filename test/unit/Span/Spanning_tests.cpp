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

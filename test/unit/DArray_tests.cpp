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

#include <complex>
#include <string>
#include <utility>
#include "DContainers/DArray.hpp"
#include "DContainers/Span.hpp"

using mdc::DArray, mdc::DSpanning, mdc::SpanSize, mdc::Span;

class DArrayTest : public ::testing::Test {
protected:
    using complex_type = std::pair<std::string, std::complex<double>>;

    void SetUp() override {
        d2Array.at(0,0) = 0.5;
        d2Array.at(0,1) = 1.5;
        d2Array.at(0,2) = 2.5;
        d2Array.at(1,0) = 3.5;
        d2Array.at(1,1) = 4.5;
        d2Array.at(1,2) = 5.5;

        s2Array.at(0,0) = "0,0";
        s2Array.at(0,1) = "0,1";
        s2Array.at(1,0) = "1,0";
        s2Array.at(1,1) = "1,1";

        i3Array = {
                {
                        {1, 2, 3},
                        {4, 5, 6}
                },
                {
                        {7, 8, 9},
                        {10, 11, 12}
                },
        };

        f1Array = { -0.1f, 15.4f, -10.9f, 0.0f, 3.14f };

        complexTypeArray = {
                { complex_type{"0,0", {0,0}}, complex_type{"0,1", {0,1}} },
                { complex_type{"1,0", {1,0}}, complex_type{"1,1", {1,1}} }
        };
    }

    DArray<double, 2, 3> d2Array;
    DArray<std::string, 2, 2> s2Array;
    DArray<int, 2, 2, 3> i3Array;
    DArray<float, 5> f1Array;
    DArray<complex_type , 2, 2> complexTypeArray;
};

TEST_F(DArrayTest, VectorTotal) {
    EXPECT_EQ(d2Array.total(), 6);
    EXPECT_EQ(s2Array.total(), 4);
    EXPECT_EQ(i3Array.total(), 12);
    EXPECT_EQ(f1Array.total(), 5);
    EXPECT_EQ(complexTypeArray.total(), 4);
}

TEST_F(DArrayTest, ElementsFetch) {
    EXPECT_EQ(d2Array.at(0,0), 0.5);
    EXPECT_EQ(d2Array.at(0,1), 1.5);
    EXPECT_EQ(d2Array.at(1,0), 3.5);
    EXPECT_EQ(d2Array.at(1,2), 5.5);

    EXPECT_EQ(s2Array.at(0,0), "0,0");
    EXPECT_EQ(s2Array.at(1,1), "1,1");

    EXPECT_EQ(i3Array.at(0,0,0),1);
    EXPECT_EQ(i3Array.at(0,1,2), 6);
    EXPECT_EQ(i3Array.at(1,0,0), 7);
    EXPECT_EQ(i3Array.at(1,1,2), 12);

    EXPECT_FLOAT_EQ(f1Array.at(0), -0.1);
    EXPECT_FLOAT_EQ(f1Array.at(1), 15.4);
    EXPECT_FLOAT_EQ(f1Array.at(4), 3.14);

    EXPECT_EQ(complexTypeArray.at(0,0).first, "0,0");
    EXPECT_EQ(complexTypeArray.at(0,0).second, std::complex<double>(0,0));
    EXPECT_EQ(complexTypeArray.at(1,1).first, "1,1");
    EXPECT_EQ(complexTypeArray.at(1,1).second, std::complex<double>(1,1));
}

TEST_F(DArrayTest, ElementsAssignment) {
    d2Array.at(0,1) = 15.2;
    EXPECT_EQ(d2Array.at(0,1), 15.2);

    s2Array.at(1,0) = "changed value";
    EXPECT_EQ(s2Array.at(1,0), "changed value");

    i3Array.at(1,0,1) = 20;
    EXPECT_EQ(i3Array.at(1,0,1), 20);

    f1Array.at(3) = -23;
    EXPECT_FLOAT_EQ(f1Array.at(3), -23);

    complexTypeArray.at(0,1) = {"key string", {12,4}};
    EXPECT_EQ(complexTypeArray.at(0,1).first, "key string");
    EXPECT_EQ(complexTypeArray.at(0,1).second, std::complex<double>(12, 4));
}

TEST_F(DArrayTest, SubvectorFetch) {
    DArray<double, 3> subD2Array = d2Array.at(1);
    EXPECT_EQ(subD2Array.at(1), 4.5);

    DArray<std::string, 2> subS2Array = s2Array.at(1);
    EXPECT_EQ(subS2Array.at(0), "1,0");

    DArray<int, 2, 3> subI3Array = i3Array.at(1);
    EXPECT_EQ(subI3Array.at(1,1), 11);

    DArray<int, 3> singleI3Array = i3Array.at(1,0);
    EXPECT_EQ(singleI3Array.at(2), 9);

    DArray<complex_type, 2> subComplexTypeArray = complexTypeArray.at(0);
    EXPECT_EQ(subComplexTypeArray.at(1).first, "0,1");
    EXPECT_EQ(subComplexTypeArray.at(1).second, std::complex<double>(0,1));
}

TEST_F(DArrayTest, SubvectorRefAssignment) {
    DArray<double, 3>& subD2Array = d2Array.at(1);
    subD2Array = {-3.5, -4.5, -5.5};
    EXPECT_EQ(subD2Array.at(1), -4.5);
    EXPECT_EQ(d2Array.at(1,1), -4.5);

    DArray<std::string, 2>& subS2Array = s2Array.at(1);
    subS2Array = { "first", "second" };
    EXPECT_EQ(subS2Array.at(1), "second");
    EXPECT_EQ(s2Array.at(1,1), "second");

    DArray<int, 2, 3>& subI3Array = i3Array.at(1);
    subI3Array = {
            {-1, -2, -3},
            {-4, -5, -6}
    };
    EXPECT_EQ(subI3Array.at(1,1), -5);
    EXPECT_EQ(i3Array.at(1,1,1), -5);

    DArray<int, 3>& singleI3Array = i3Array.at(0,0);
    singleI3Array = { -20, -21 };
    EXPECT_EQ(singleI3Array.at(0), -20);
    EXPECT_EQ(i3Array.at(0,0,0), -20);

    DArray<complex_type, 2>& subComplexTypeArray = complexTypeArray.at(0);
    subComplexTypeArray = {complex_type{"first key", {-1, -1}}, complex_type{"second key", {-2, -2}} };
    EXPECT_EQ(subComplexTypeArray.at(1).first, "second key");
    EXPECT_EQ(subComplexTypeArray.at(1).second, std::complex<double>(-2, -2));
    EXPECT_EQ(complexTypeArray.at(0,1).first, "second key");
    EXPECT_EQ(complexTypeArray.at(0,1).second, std::complex<double>(-2,-2));
}

TEST_F(DArrayTest, SpanViewMethods) {
    DArray<int, 2, 1, 2> spanI3Array = i3Array.at(DSpanning<SpanSize::All>(), DSpanning<SpanSize::Index<1>>(), DSpanning<SpanSize::Interval<1,2>>());
    DArray<int, 2, 1, 2> spanI3ArrayWrap = i3Array.at(Span::all(), Span::of<1>(), Span::of<2>(1, 2));
    DArray<int, 2, 1, 2> expectedViewI3Array = {
            {
                    {5, 6}
            },
            {
                    {11, 12}
            }
    };
    EXPECT_EQ(spanI3Array, expectedViewI3Array);
    EXPECT_EQ(spanI3Array, spanI3ArrayWrap);

    DArray<float, 2> spanF1Array = f1Array.at(DSpanning<SpanSize::Interval<2>>(1, 2));
    DArray<float, 2> spanF1ArrayWrap = f1Array.at(Span::of<1,2>());
    DArray<float, 2> expectedViewF1Array = {15.4f, -10.9f};
    EXPECT_EQ(spanF1Array, expectedViewF1Array);
    EXPECT_EQ(spanF1Array, spanF1ArrayWrap);

    DArray<std::string, 1, 1> spanS2Array = s2Array.at(DSpanning<SpanSize::Index<1>>(), DSpanning<SpanSize::Index<1>>());
    DArray<std::string, 1, 1> spanS2ArrayWrap = s2Array.at(Span::of<1>(), Span::of<1>());
    DArray<std::string, 1, 1> expectedViewS2Array = {{"1,1"}};
    EXPECT_EQ(spanS2Array, expectedViewS2Array);
    EXPECT_EQ(spanS2Array, spanS2ArrayWrap);
}

TEST_F(DArrayTest, AllSpanView) {
    auto spanI3Array = i3Array.at(DSpanning<SpanSize::All>(), DSpanning<SpanSize::All>(), DSpanning<SpanSize::All>());
    EXPECT_EQ(spanI3Array, i3Array);

    auto spanF1Array = f1Array.at(DSpanning<SpanSize::All>());
    EXPECT_EQ(spanF1Array, f1Array);

    auto spanS2Array = s2Array.at(DSpanning<SpanSize::All>(), DSpanning<SpanSize::All>());
    EXPECT_EQ(spanS2Array, s2Array);
}


TEST_F(DArrayTest, ReadMeTest) {
    DArray<double, 2, 3> matrix = {
            {4.2, 11., -1.5},
            {0.0, 1.0, 3.33}
    };
    matrix.at(0,2) = 2.1;
    DArray<double, 3>& subMatrix = matrix.at(1);
    subMatrix.at(2) = -3.33;
    auto viewMatrix = matrix.at(Span::all(), Span::of<0>());
    DArray<double, 2, 1> testView = {{4.2},{0.0}};
    EXPECT_EQ(viewMatrix, testView);
    EXPECT_EQ(viewMatrix.total(),2);
}

TEST_F(DArrayTest, ArrayPrinting) {
    // suppress console output
    auto console = std::cout.rdbuf(nullptr);

    std::cout << f1Array << std::endl;
    std::cout << d2Array << std::endl;
    std::cout << i3Array << std::endl;

    // restore console output
    std::cout.rdbuf(console);
}

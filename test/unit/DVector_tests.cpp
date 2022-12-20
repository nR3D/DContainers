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
#include "DContainers/DVector.hpp"
#include "DContainers/Span.hpp"

using mdc::DVector, mdc::Spanning, mdc::Span;

class DVectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        d2Vector.at(0,0) = 0.5;
        d2Vector.at(0,1) = 1.5;
        d2Vector.at(0,2) = 2.5;
        d2Vector.at(1,0) = 3.5;
        d2Vector.at(1,1) = 4.5;
        d2Vector.at(1,2) = 5.5;

        s2Vector.at(0,0) = "0,0";
        s2Vector.at(0,1) = "0,1";
        s2Vector.at(1,0) = "1,0";
        s2Vector.at(1,1) = "1,1";

        i3Vector = {
                        {
                                {1, 2, 3},
                                {4, 5, 6, 7}
                        },
                        {
                                {8, 9},
                                {10, 11, 12, 13, 14}
                        },
                    };

        f1Vector = { -0.1, 15.4, -10.9, 0.0, 3.14 };

        complexTypeVector = {
                { {"0,0", {0,0}}, {"0,1", {0,1}} },
                { {"1,0", {1,0}}, {"1,1", {1,1}} }
        };
    }

    DVector<2, double> d2Vector{2,3};
    DVector<2, std::string> s2Vector{2,2};
    DVector<3, int> i3Vector;
    DVector<1, float> f1Vector;
    DVector<2, std::pair<std::string, std::complex<double>>> complexTypeVector;
};

TEST_F(DVectorTest, VectorTotal) {
    EXPECT_EQ(d2Vector.total(), 6);
    EXPECT_EQ(s2Vector.total(), 4);
    EXPECT_EQ(i3Vector.total(), 14);
    EXPECT_EQ(f1Vector.total(), 5);
    EXPECT_EQ(complexTypeVector.total(), 4);
}

TEST_F(DVectorTest, ElementsFetch) {
    EXPECT_EQ(d2Vector.at(0,0), 0.5);
    EXPECT_EQ(d2Vector.at(0,1), 1.5);
    EXPECT_EQ(d2Vector.at(1,0), 3.5);
    EXPECT_EQ(d2Vector.at(1,2), 5.5);

    EXPECT_EQ(s2Vector.at(0,0), "0,0");
    EXPECT_EQ(s2Vector.at(1,1), "1,1");

    EXPECT_EQ(i3Vector.at(0,0,0),1);
    EXPECT_EQ(i3Vector.at(0,1,2), 6);
    EXPECT_EQ(i3Vector.at(1,0,0), 8);
    EXPECT_EQ(i3Vector.at(1,1,4), 14);

    EXPECT_FLOAT_EQ(f1Vector.at(0), -0.1);
    EXPECT_FLOAT_EQ(f1Vector.at(1), 15.4);
    EXPECT_FLOAT_EQ(f1Vector.at(4), 3.14);

    EXPECT_EQ(complexTypeVector.at(0,0).first, "0,0");
    EXPECT_EQ(complexTypeVector.at(0,0).second, std::complex<double>(0,0));
    EXPECT_EQ(complexTypeVector.at(1,1).first, "1,1");
    EXPECT_EQ(complexTypeVector.at(1,1).second, std::complex<double>(1,1));
}

TEST_F(DVectorTest, ElementsAssignment) {
    d2Vector.at(0,1) = 15.2;
    EXPECT_EQ(d2Vector.at(0,1), 15.2);

    s2Vector.at(1,0) = "changed value";
    EXPECT_EQ(s2Vector.at(1,0), "changed value");

    i3Vector.at(1,0,1) = 20;
    EXPECT_EQ(i3Vector.at(1,0,1), 20);

    f1Vector.at(3) = -23;
    EXPECT_FLOAT_EQ(f1Vector.at(3), -23);

    complexTypeVector.at(0,1) = {"key string", {12,4}};
    EXPECT_EQ(complexTypeVector.at(0,1).first, "key string");
    EXPECT_EQ(complexTypeVector.at(0,1).second, std::complex<double>(12, 4));
}

TEST_F(DVectorTest, SubvectorFetch) {
    DVector<1, double> subD2Vector = d2Vector.at(1);
    EXPECT_EQ(subD2Vector.at(1), 4.5);

    DVector<1, std::string> subS2Vector = s2Vector.at(1);
    EXPECT_EQ(subS2Vector.at(0), "1,0");

    DVector<2, int> subI3Vector = i3Vector.at(1);
    EXPECT_EQ(subI3Vector.at(1,3), 13);

    DVector<1, int> singleI3Vector = i3Vector.at(1,0);
    EXPECT_EQ(singleI3Vector.at(1), 9);

    DVector<1, std::pair<std::string, std::complex<double>>> subComplexTypeVector = complexTypeVector.at(0);
    EXPECT_EQ(subComplexTypeVector.at(1).first, "0,1");
    EXPECT_EQ(subComplexTypeVector.at(1).second, std::complex<double>(0,1));
}

TEST_F(DVectorTest, SubvectorRefAssignment) {
    DVector<1, double>& subD2Vector = d2Vector.at(1);
    subD2Vector = {-3.5, -4.5, -5.5};
    EXPECT_EQ(subD2Vector.at(1), -4.5);
    EXPECT_EQ(d2Vector.at(1,1), -4.5);

    DVector<1, std::string>& subS2Vector = s2Vector.at(1);
    subS2Vector = { "first", "second" };
    EXPECT_EQ(subS2Vector.at(1), "second");
    EXPECT_EQ(s2Vector.at(1,1), "second");

    DVector<2, int>& subI3Vector = i3Vector.at(1);
    subI3Vector = {
            {-1, -2, -3},
            {-4, -5, -6}
    };
    EXPECT_EQ(subI3Vector.at(1,1), -5);
    EXPECT_EQ(i3Vector.at(1,1,1), -5);

    DVector<1, int>& singleI3Vector = i3Vector.at(0,0);
    singleI3Vector = { -20, -21 };
    EXPECT_EQ(singleI3Vector.at(0), -20);
    EXPECT_EQ(i3Vector.at(0,0,0), -20);

    DVector<1, std::pair<std::string, std::complex<double>>>& subComplexTypeVector = complexTypeVector.at(0);
    subComplexTypeVector = {{"first key", {-1, -1}}, {"second key", {-2, -2}}};
    EXPECT_EQ(subComplexTypeVector.at(1).first, "second key");
    EXPECT_EQ(subComplexTypeVector.at(1).second, std::complex<double>(-2,-2));
    EXPECT_EQ(complexTypeVector.at(0,1).first, "second key");
    EXPECT_EQ(complexTypeVector.at(0,1).second, std::complex<double>(-2,-2));
}

TEST_F(DVectorTest, SpanViewMethods) {
    DVector<3, int> spanI3Vector = i3Vector.at(Spanning::all(), Spanning(1), Spanning(1, 2));
    DVector<3, int> expectedViewI3Vector = {
            {
                {5, 6}
            },
            {
                {11, 12}
            }
    };
    EXPECT_EQ(spanI3Vector, expectedViewI3Vector);

    DVector<1, float> spanF1Vector = f1Vector.at(Spanning(1, 2));
    DVector<1, float> expectedViewF1Vector = {15.4, -10.9};
    EXPECT_EQ(spanF1Vector, expectedViewF1Vector);

    DVector<2, std::string> spanS2Vector = s2Vector.at(Spanning(1), Spanning(1));
    DVector<2, std::string> expectedViewS2Vector = {{"1,1"}};
    EXPECT_EQ(spanS2Vector, expectedViewS2Vector);
}

TEST_F(DVectorTest, AllSpanView) {
    DVector<3, int> spanI3Vector = i3Vector.at(Spanning::all(), Spanning::all(), Spanning::all());
    EXPECT_EQ(spanI3Vector, i3Vector);

    DVector<1, float> spanF1Vector = f1Vector.at(Spanning::all());
    EXPECT_EQ(spanF1Vector, f1Vector);

    DVector<2, std::string> spanS2Vector = s2Vector.at(Spanning::all(), Spanning::all());
    EXPECT_EQ(spanS2Vector, s2Vector);
}

TEST_F(DVectorTest, SpanWrapperView) {
    DVector<3, int> spanI3VectorRT = i3Vector.at(Span::all(), Span::of(1), Span::of(1, 2));
    DVector<3, int> spanI3VectorCT = i3Vector.at(Span::all(), Span::of<1>(), Span::of<1,2>());
    DVector<3, int> expectedViewI3Vector = {
            {
                    {5, 6}
            },
            {
                    {11, 12}
            }
    };
    EXPECT_EQ(spanI3VectorRT, expectedViewI3Vector);
    EXPECT_EQ(spanI3VectorRT, spanI3VectorCT);

    DVector<1, float> spanF1VectorRT = f1Vector.at(Span::of(1, 2));
    DVector<1, float> spanF1VectorCT = f1Vector.at(Span::of<1,2>());
    DVector<1, float> expectedViewF1Vector = {15.4, -10.9};
    EXPECT_EQ(spanF1VectorRT, expectedViewF1Vector);
    EXPECT_EQ(spanF1VectorRT, spanF1VectorCT);

    DVector<2, std::string> spanS2VectorRT = s2Vector.at(Span::of(1), Span::of(1));
    DVector<2, std::string> spanS2VectorCT = s2Vector.at(Span::of<1>(), Span::of<1>());
    DVector<2, std::string> expectedViewS2Vector = {{"1,1"}};
    EXPECT_EQ(spanS2VectorRT, expectedViewS2Vector);
    EXPECT_EQ(spanS2VectorRT, spanS2VectorCT);
}

TEST_F(DVectorTest, VectorPrinting) {
    // suppress console output
    auto console = std::cout.rdbuf(nullptr);

    std::cout << f1Vector << std::endl;
    std::cout << d2Vector << std::endl;
    std::cout << i3Vector << std::endl;

    // restore console output
    std::cout.rdbuf(console);
}
#include <gtest/gtest.h>
#include "../src/DVector.hpp"

#include <complex>
#include <string>
#include <utility>

class DVectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        d2Vector(0,0) = 0.5;
        d2Vector(0,1) = 1.5;
        d2Vector(0,2) = 2.5;
        d2Vector(1,0) = 3.5;
        d2Vector(1,1) = 4.5;
        d2Vector(1,2) = 5.5;

        s2Vector(0,0) = "0,0";
        s2Vector(0,1) = "0,1";
        s2Vector(1,0) = "1,0";
        s2Vector(1,1) = "1,1";

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
    EXPECT_EQ(d2Vector(0,0), 0.5);
    EXPECT_EQ(d2Vector(0,1), 1.5);
    EXPECT_EQ(d2Vector(1,0), 3.5);
    EXPECT_EQ(d2Vector(1,2), 5.5);

    EXPECT_EQ(s2Vector(0,0), "0,0");
    EXPECT_EQ(s2Vector(1,1), "1,1");

    EXPECT_EQ(i3Vector(0,0,0),1);
    EXPECT_EQ(i3Vector(0,1,2), 6);
    EXPECT_EQ(i3Vector(1,0,0), 8);
    EXPECT_EQ(i3Vector(1,1,4), 14);

    EXPECT_FLOAT_EQ(f1Vector(0), -0.1);
    EXPECT_FLOAT_EQ(f1Vector(1), 15.4);
    EXPECT_FLOAT_EQ(f1Vector(4), 3.14);

    EXPECT_EQ(complexTypeVector(0,0).first, "0,0");
    EXPECT_EQ(complexTypeVector(0,0).second, std::complex<double>(0,0));
    EXPECT_EQ(complexTypeVector(1,1).first, "1,1");
    EXPECT_EQ(complexTypeVector(1,1).second, std::complex<double>(1,1));
}

TEST_F(DVectorTest, ElementsAssignment) {
    d2Vector(0,1) = 15.2;
    EXPECT_EQ(d2Vector(0,1), 15.2);

    s2Vector(1,0) = "changed value";
    EXPECT_EQ(s2Vector(1,0), "changed value");

    i3Vector(1,0,1) = 20;
    EXPECT_EQ(i3Vector(1,0,1), 20);

    f1Vector(3) = -23;
    EXPECT_FLOAT_EQ(f1Vector(3), -23);

    complexTypeVector(0,1) = {"key string", {12,4}};
    EXPECT_EQ(complexTypeVector(0,1).first, "key string");
    EXPECT_EQ(complexTypeVector(0,1).second, std::complex<double>(12, 4));
}

TEST_F(DVectorTest, SubvectorFetch) {
    DVector<1, double> subD2Vector = d2Vector(1);
    EXPECT_EQ(subD2Vector(1), 4.5);

    DVector<1, std::string> subS2Vector = s2Vector(1);
    EXPECT_EQ(subS2Vector(0), "1,0");

    DVector<2, int> subI3Vector = i3Vector(1);
    EXPECT_EQ(subI3Vector(1,3), 13);

    DVector<1, int> singleI3Vector = i3Vector(1,0);
    EXPECT_EQ(singleI3Vector(1), 9);

    DVector<1, std::pair<std::string, std::complex<double>>> subComplexTypeVector = complexTypeVector(0);
    EXPECT_EQ(subComplexTypeVector(1).first, "0,1");
    EXPECT_EQ(subComplexTypeVector(1).second, std::complex<double>(0,1));
}

TEST_F(DVectorTest, SubvectorRefAssignment) {
    DVector<1, double>& subD2Vector = d2Vector(1);
    subD2Vector = {-3.5, -4.5, -5.5};
    EXPECT_EQ(subD2Vector(1), -4.5);
    EXPECT_EQ(d2Vector(1,1), -4.5);

    DVector<1, std::string>& subS2Vector = s2Vector(1);
    subS2Vector = { "first", "second" };
    EXPECT_EQ(subS2Vector(1), "second");
    EXPECT_EQ(s2Vector(1,1), "second");

    DVector<2, int>& subI3Vector = i3Vector(1);
    subI3Vector = {
            {-1, -2, -3},
            {-4, -5, -6}
    };
    EXPECT_EQ(subI3Vector(1,1), -5);
    EXPECT_EQ(i3Vector(1,1,1), -5);

    DVector<1, int>& singleI3Vector = i3Vector(0,0);
    singleI3Vector = { -20, -21 };
    EXPECT_EQ(singleI3Vector(0), -20);
    EXPECT_EQ(i3Vector(0,0,0), -20);

    DVector<1, std::pair<std::string, std::complex<double>>>& subComplexTypeVector = complexTypeVector(0);
    subComplexTypeVector = {{"first key", {-1, -1}}, {"second key", {-2, -2}}};
    EXPECT_EQ(subComplexTypeVector(1).first, "second key");
    EXPECT_EQ(subComplexTypeVector(1).second, std::complex<double>(-2,-2));
    EXPECT_EQ(complexTypeVector(0,1).first, "second key");
    EXPECT_EQ(complexTypeVector(0,1).second, std::complex<double>(-2,-2));
}

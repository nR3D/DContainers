#include <gtest/gtest.h>
#include "../src/DArray.hpp"

#include <complex>
#include <string>
#include <utility>

class DArrayTest : public ::testing::Test {
protected:
    using complex_type = std::pair<std::string, std::complex<double>>;

    void SetUp() override {
        d2Array(0,0) = 0.5;
        d2Array(0,1) = 1.5;
        d2Array(0,2) = 2.5;
        d2Array(1,0) = 3.5;
        d2Array(1,1) = 4.5;
        d2Array(1,2) = 5.5;

        s2Array(0,0) = "0,0";
        s2Array(0,1) = "0,1";
        s2Array(1,0) = "1,0";
        s2Array(1,1) = "1,1";

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
    EXPECT_EQ(d2Array(0,0), 0.5);
    EXPECT_EQ(d2Array(0,1), 1.5);
    EXPECT_EQ(d2Array(1,0), 3.5);
    EXPECT_EQ(d2Array(1,2), 5.5);

    EXPECT_EQ(s2Array(0,0), "0,0");
    EXPECT_EQ(s2Array(1,1), "1,1");

    EXPECT_EQ(i3Array(0,0,0),1);
    EXPECT_EQ(i3Array(0,1,2), 6);
    EXPECT_EQ(i3Array(1,0,0), 7);
    EXPECT_EQ(i3Array(1,1,2), 12);

    EXPECT_FLOAT_EQ(f1Array(0), -0.1);
    EXPECT_FLOAT_EQ(f1Array(1), 15.4);
    EXPECT_FLOAT_EQ(f1Array(4), 3.14);

    EXPECT_EQ(complexTypeArray(0,0).first, "0,0");
    EXPECT_EQ(complexTypeArray(0,0).second, std::complex<double>(0,0));
    EXPECT_EQ(complexTypeArray(1,1).first, "1,1");
    EXPECT_EQ(complexTypeArray(1,1).second, std::complex<double>(1,1));
}

TEST_F(DArrayTest, ElementsAssignment) {
    d2Array(0,1) = 15.2;
    EXPECT_EQ(d2Array(0,1), 15.2);

    s2Array(1,0) = "changed value";
    EXPECT_EQ(s2Array(1,0), "changed value");

    i3Array(1,0,1) = 20;
    EXPECT_EQ(i3Array(1,0,1), 20);

    f1Array(3) = -23;
    EXPECT_FLOAT_EQ(f1Array(3), -23);

    complexTypeArray(0,1) = {"key string", {12,4}};
    EXPECT_EQ(complexTypeArray(0,1).first, "key string");
    EXPECT_EQ(complexTypeArray(0,1).second, std::complex<double>(12, 4));
}

TEST_F(DArrayTest, SubvectorFetch) {
    DArray<double, 3> subD2Array = d2Array(1);
    EXPECT_EQ(subD2Array(1), 4.5);

    DArray<std::string, 2> subS2Array = s2Array(1);
    EXPECT_EQ(subS2Array(0), "1,0");

    DArray<int, 2, 3> subI3Array = i3Array(1);
    EXPECT_EQ(subI3Array(1,1), 11);

    DArray<int, 3> singleI3Array = i3Array(1,0);
    EXPECT_EQ(singleI3Array(2), 9);

    DArray<complex_type, 2> subComplexTypeArray = complexTypeArray(0);
    EXPECT_EQ(subComplexTypeArray(1).first, "0,1");
    EXPECT_EQ(subComplexTypeArray(1).second, std::complex<double>(0,1));
}

TEST_F(DArrayTest, SubvectorRefAssignment) {
    DArray<double, 3>& subD2Array = d2Array(1);
    subD2Array = {-3.5, -4.5, -5.5};
    EXPECT_EQ(subD2Array(1), -4.5);
    EXPECT_EQ(d2Array(1,1), -4.5);

    DArray<std::string, 2>& subS2Array = s2Array(1);
    subS2Array = { "first", "second" };
    EXPECT_EQ(subS2Array(1), "second");
    EXPECT_EQ(s2Array(1,1), "second");

    DArray<int, 2, 3>& subI3Array = i3Array(1);
    subI3Array = {
            {-1, -2, -3},
            {-4, -5, -6}
    };
    EXPECT_EQ(subI3Array(1,1), -5);
    EXPECT_EQ(i3Array(1,1,1), -5);

    DArray<int, 3>& singleI3Array = i3Array(0,0);
    singleI3Array = { -20, -21 };
    EXPECT_EQ(singleI3Array(0), -20);
    EXPECT_EQ(i3Array(0,0,0), -20);

    DArray<complex_type, 2>& subComplexTypeArray = complexTypeArray(0);
    subComplexTypeArray = {complex_type{"first key", {-1, -1}}, complex_type{"second key", {-2, -2}} };
    EXPECT_EQ(subComplexTypeArray(1).first, "second key");
    EXPECT_EQ(subComplexTypeArray(1).second, std::complex<double>(-2, -2));
    EXPECT_EQ(complexTypeArray(0,1).first, "second key");
    EXPECT_EQ(complexTypeArray(0,1).second, std::complex<double>(-2,-2));
}

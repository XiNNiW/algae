#include <gtest/gtest.h>
#include "../../src/core/simd/simd.h"
#include <cmath>

TEST(SIMD_Test, vector2db_and) {
    vector2db av, bv, cv;
    av = vector2db(true, true);
    bv = vector2db(true, false);
    bool expected[2]  = {true, false};
    bool actual[2];
    cv = av & bv;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector2db_or) {

    vector2db av, bv, cv;
    av = vector2db(true, true );
    bv = vector2db(true, false);
    bool expected[2]  = {true, true};
    bool actual[2];
    cv = av | bv;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector2db_xor) {

    vector2db av, bv, cv;
    av = vector2db(true, true );
    bv = vector2db(true, false);
    bool expected[2]  = {false, true};
    bool actual[2];
    cv = av ^ bv;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector2db_not) {

    vector2db av, bv, cv;
    av = vector2db(true, true);
    bool expected[2]  = {false, false};
    bool actual[2];
    cv = !av;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
#include <gtest/gtest.h>
#include "../../src/core/simd.h"
#include <cmath>

using algae::dsp::core::simd::vector4fb;

TEST(SIMD_Test, vector4fb_and) {
    vector4fb av, bv, cv;
    av = vector4fb(true, true,  false, false);
    bv = vector4fb(true, false, true,  false);
    bool expected[4]  = {true, false, false, false};
    bool actual[4];
    cv = av & bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4fb_or) {

    vector4fb av, bv, cv;
    av = vector4fb(true, true,  false, false);
    bv = vector4fb(true, false, true,  false);
    bool expected[4]  = {true, true, true, false};
    bool actual[4];
    cv = av | bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4fb_xor) {

    vector4fb av, bv, cv;
    av = vector4fb(true, true,  false, false);
    bv = vector4fb(true, false, true,  false);
    bool expected[4]  = {false, true, true, false};
    bool actual[4];
    cv = av ^ bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4fb_not) {

    vector4fb av, bv, cv;
    av = vector4fb(true, true,  false, false);
    bool expected[4]  = {false, false, true, true};
    bool actual[4];
    cv = !av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
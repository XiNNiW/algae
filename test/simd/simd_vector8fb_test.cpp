#include "../../src/core/simd/simd_config.h"
#if SSE_INSTR_SET>6
#include <gtest/gtest.h>
#include "../../src/core/simd/simd.h"
#include <cmath>

TEST(SIMD_Test, vector8fb_and) {
    vector8fb av, bv, cv;
    av = vector8fb(true, true,  false, false,true, true,  false, false);
    bv = vector8fb(true, false, true,  false,true, false, true,  false);
    bool expected[8]  = {true, false, false, false,true, false, false, false};
    bool actual[8];
    cv = av & bv;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector8fb_or) {

    vector8fb av, bv, cv;
    av = vector8fb(true, true,  false, false,true, true,  false, false);
    bv = vector8fb(true, false, true,  false,true, false, true,  false);
    bool expected[8]  = {true, true, true, false,true, true, true, false};
    bool actual[8];
    cv = av | bv;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector8fb_xor) {

    vector8fb av, bv, cv;
    av = vector8fb(true, true,  false, false,true, true,  false, false);
    bv = vector8fb(true, false, true,  false,true, false, true,  false);
    bool expected[8]  = {false, true, true, false, false, true, true, false};
    bool actual[8];
    cv = av ^ bv;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector8fb_not) {

    vector8fb av, bv, cv;
    av = vector8fb(true, true,  false, false,true, true,  false, false);
    bool expected[8]  = {false, false, true, true,false, false, true, true};
    bool actual[8];
    cv = !av;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
#endif
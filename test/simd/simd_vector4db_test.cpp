#include "../../src/core/simd/simd_config.h"
#if SSE_INSTR_SET>6
#include <gtest/gtest.h>
#include "../../src/core/simd/simd.h"
#include <cmath>

TEST(SIMD_Test, vector4db_and) {
    vector4db av, bv, cv;
    av = vector4db(true, true,  false, false);
    bv = vector4db(true, false, true,  false);
    bool expected[4]  = {true, false, false, false};
    bool actual[4];
    cv = av & bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4db_or) {

    vector4db av, bv, cv;
    av = vector4db(true, true,  false, false);
    bv = vector4db(true, false, true,  false);
    bool expected[4]  = {true, true, true, false};
    bool actual[4];
    cv = av | bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4db_xor) {

    vector4db av, bv, cv;
    av = vector4db(true, true,  false, false);
    bv = vector4db(true, false, true,  false);
    bool expected[4]  = {false, true, true, false};
    bool actual[4];
    cv = av ^ bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4db_not) {

    vector4db av, bv, cv;
    av = vector4db(true, true,  false, false);
    bool expected[4]  = {false, false, true, true};
    bool actual[4];
    cv = !av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
#endif
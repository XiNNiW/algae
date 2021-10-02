#include <gtest/gtest.h>
#include <iostream>
#include "../../src/core/math_functions.h"
#include "../../src/core/audio_block.h"
#include "../../src/core/oscillator.h"

using algae::dsp::core::AudioBlock;


// 


using algae::dsp::core::oscillator::factorial_t;
TEST(DSP_Test, CORE_factorial_t) {
    double expected;
    double actual;

    expected = 1;
    actual = factorial_t<double,1>::result;
    EXPECT_FLOAT_EQ(expected, actual);

    expected = 2;
    actual = factorial_t<double,2>::result;
    EXPECT_FLOAT_EQ(expected, actual);

    expected = 3*2*1;
    actual = factorial_t<double,3>::result;
    EXPECT_FLOAT_EQ(expected, actual);

    expected = 4*3*2*1;
    actual = factorial_t<double,4>::result;
    EXPECT_FLOAT_EQ(expected, actual);
}



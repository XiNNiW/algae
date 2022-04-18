#include <gtest/gtest.h>
#include "../../src/core/filter/dc_blocker.h"
#include <math.h>
#include "../frequency_test_utilities.h"

using algae::dsp::core::filter::dc_block_t;
using algae::dsp::core::filter::process;
TEST(Filter_Test, CORE_DC_blocker_blocks_DC_offset) { 

    dc_block_t<double> filter = dc_block_t<double>();
    const double DCoffset = 100;

    for(int i=0; i<1000; i++){
        double input = DCoffset + sin(3.14159*i*2020.0);
        filter = process<double>(filter, input);
    }

    EXPECT_LT(filter.y1,2);
}

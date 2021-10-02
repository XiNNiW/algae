#include <gtest/gtest.h>
#include "../../src/core/filter/dc_blocker.h"
#include <math.h>

using algae::dsp::core::filter::dc_block_t;
using algae::dsp::core::filter::process_dc_blocker;
TEST(DSP_Test, CORE_DC_blocker_blocks_DC_offset) { 
    double accum;
    dc_block_t<double> filter = dc_block_t<double>();

    auto accumulateDC = [&](){
        accum += 100.1;
    };

    for(int i=0; i<1000; i++){
        accumulateDC();
        double input = accum + sin(3.14159*i*2020.0);
        filter = process_dc_blocker<double>(filter, input);
    }

    EXPECT_FLOAT_EQ(0, filter.y1);
}

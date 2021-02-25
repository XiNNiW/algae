#include <gtest/gtest.h>
#include "../src/core/dc_blocker.h"
#include "../src/core/integrator.h"
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

using algae::dsp::core::filter::integrator_t;
using algae::dsp::core::filter::process_integrator;
TEST(DSP_Test, CORE_DC_blocker_blocks_DC_offset_from_integrator) { 
    double input[10] = {1,0,0,0,1,0,0,0,1,0};
    double output[10] = {};
    integrator_t<double> integrator = integrator_t<double>();
    dc_block_t<double> dc_blocker = dc_block_t<double>();

    for(int i=0; i<10; i++){
        integrator = process_integrator<double>(integrator, input[i]);
        dc_blocker = process_dc_blocker<double>(dc_blocker, integrator.y1);
        output[i] = dc_blocker.y1;
        EXPECT_LT(output[i],1.0001);
        EXPECT_GT(output[i],-1.0001);
    }

    
}
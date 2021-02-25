#include <gtest/gtest.h>
#include "../src/core/integrator.h"
#include <math.h>

using algae::dsp::core::filter::integrator_t;
using algae::dsp::core::filter::process_integrator;
TEST(DSP_Test, CORE_integrator_accumulates_signal) { 
    double accum;
    integrator_t<double> filter = integrator_t<double>();
    double input[10] = {1,1,1,0,0,0,0,0,0,0};
    double expected[10] = {1,2,3,3,3,3,3,3,3,3};
    double output[10] = {};

    for(int i=0; i<10; i++){
        filter = process_integrator(filter,input[i]);
        output[i] = filter.y1;
        EXPECT_FLOAT_EQ(expected[i], output[i]);
    }

}

using algae::dsp::core::filter::leaky_integrator_t;
using algae::dsp::core::filter::leaky_integrator;
using algae::dsp::core::filter::process_leaky_integrator;
TEST(DSP_Test, CORE_leaky_integrator_accumulates_signal_above_cutoffhz) { 
    double accum;
    double cutoff= 20.0;
    double sampleRate= 48000;
    leaky_integrator_t<double> filter = leaky_integrator<double,double>(leaky_integrator_t<double>(),cutoff,sampleRate);
    double input[10] = {1,1,1,0,0,0,0,0,0,0};
    double expected[10] = {0,0,0,0,0,0,0,0,0,0};
    double output[10] = {};

    for(int i=0; i<10; i++){
        filter = process_leaky_integrator(filter,input[i]);
        output[i] = filter.y1;
        EXPECT_FLOAT_EQ(expected[i], output[i]);
    }

}
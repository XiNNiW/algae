#include <gtest/gtest.h>
#include "../../src/shell/oscillator.h"
#include "../../src/shell/filter.h"
#include <iostream>

using algae::dsp::core::AudioBlock;
using algae::dsp::core::filter::process;
using algae::dsp::core::oscillator::noise;


using algae::dsp::core::filter::biquad_t;
using algae::dsp::core::filter::lowpass;

using algae::dsp::core::filter::chaotic_resonator_t;
using algae::dsp::core::filter::update_coefficients;
// using algae::dsp::core::filter::process_chaos;
using algae::dsp::core::oscillator::cos_t;

TEST(DSP_Test, CORE_chaotic_cos_bandpass) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    chaotic_resonator_t<double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    //NOTE: for fairly normal values of gain parameters and normal signals, the reson algorithm will clip
    //is there a better way to structure the parameters to prevent this?
    //some filter designs divide out the gain introduced by the peak... however... sometimes you want a boost
    filter.resonator = update_coefficients<double,double>(filter.resonator, 220.0, 0.5, 0.25, 48000.0);
    filter.chaos_gain = 0.5;
    filter.feedback_amt = 1.0;

    for(int i = 0; i<BLOCKSIZE; i++){
        input[i] = 0.125*noise<double>();
    }

    std::tie(filter,output) = process<double,cos_t<double,0>::lookup,BLOCKSIZE>(filter, input);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
    
}
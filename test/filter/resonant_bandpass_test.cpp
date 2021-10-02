#include <gtest/gtest.h>
#include "../../src/core/audio_block.h"
#include "../../src/core/oscillator.h"
#include "../../src/core/filter.h"
#include <iostream>

using algae::dsp::core::filter::reson_bp_t;
using algae::dsp::core::filter::update_coefficients;
using algae::dsp::core::filter::process;
using algae::dsp::core::oscillator::noise;
using algae::dsp::core::AudioBlock;

TEST(DSP_Test, CORE_reson_bandpass) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    reson_bp_t<double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    //NOTE: for fairly normal values of gain parameters and normal signals, the reson algorithm will clip
    //is there a better way to structure the parameters to prevent this?
    //some filter designs divide out the gain introduced by the peak... however... sometimes you want a boost
    filter = update_coefficients<double,double>(filter, 220.0, 0.5, 0.25, 48000.0);

    for(int i = 0; i<BLOCKSIZE; i++){
        input[i] = 0.125*noise<double>();
    }

    std::tie(filter,output) = process<double>(filter, input);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
    
}
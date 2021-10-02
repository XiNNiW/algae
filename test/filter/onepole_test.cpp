#include <gtest/gtest.h>
#include "../../src/shell/dsp_graph.h"
#include "../../src/shell/oscillator.h"
#include "../../src/shell/filter.h"
#include <iostream>

using algae::dsp::core::AudioBlock;
using algae::dsp::core::filter::process;
using algae::dsp::core::oscillator::noise;



using algae::dsp::core::filter::onepole_hip_t;
using algae::dsp::core::filter::hip;

TEST(DSP_Test, CORE_onepole_hip) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    onepole_hip_t<double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    filter = hip<double,double>(filter, 220.0, 48000.0);

    for(int i = 0; i<BLOCKSIZE; i++){
        input[i] = noise<double>();
    }

    std::tie(filter,output) = process<double,double>(filter, input);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
    
}

using algae::dsp::core::filter::onepole_lop_t;
using algae::dsp::core::filter::lop;

TEST(DSP_Test, CORE_onepole_lop) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    onepole_lop_t<double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    filter = lop<double,double>(filter, 220.0, 48000.0);

    for(int i = 0; i<BLOCKSIZE; i++){
        input[i] = noise<double>();
    }

    std::tie(filter,output) = process<double,double>(filter, input);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
    
}








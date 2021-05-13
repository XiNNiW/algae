#include <gtest/gtest.h>
#include "../src/shell/dsp_graph.h"
#include "../src/shell/math_functions.h"
#include "../src/core/math_functions.h"
#include "../src/core/audio_block.h"
#include "../src/core/oscillator.h"
#include <iostream>

using algae::dsp::shell::dsp_node;
using algae::dsp::shell::connect;
using algae::dsp::shell::math::constant_node;

TEST(DSP_Test, ConstantNode) { 
    auto number = constant_node<double,double>(5);

    ASSERT_EQ(5,number.getOutputValue(0));

    number.update();

    ASSERT_EQ(5,number.getOutputValue(0));

}

using algae::dsp::core::AudioBlock;
using algae::dsp::core::math::tanh_approx_pade;
using algae::dsp::core::oscillator::noise;
TEST(DSP_Test, block_tanh_approximation) { 
    constexpr auto BLOCKSIZE = 64;
    constexpr auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = noise<float, BLOCKSIZE>();
    AudioBlock<float,BLOCKSIZE> output = tanh_approx_pade<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_GT( output[idx],-1.01);
        EXPECT_LT( output[idx],1.01);
    }

}


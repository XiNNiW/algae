#include <gtest/gtest.h>
#include <iostream>
#include "../src/shell/dsp_graph.h"
#include "../src/shell/math_functions.h"
#include "../src/shell/oscillator.h"
#include "../src/core/audio_block.h"
#include "../src/core/oscillator.h"

using algae::dsp::core::AudioBlock;
using algae::dsp::shell::dsp_node;
using algae::dsp::shell::connect;
using algae::dsp::shell::math::constant_node;
using algae::dsp::shell::oscillator::phasor;
using algae::dsp::shell::oscillator::osc;

// 



using algae::dsp::core::oscillator::makeSineTable;
// using algae::dsp::core::oscillator::makeSineTable_debug;
const std::array<double, 4> sine_4 = makeSineTable<double, 4>(); 
const std::array<double, 4> expected_sine_4 = {0, 1, 0, -1}; 
//sine wavetable
TEST(DSP_Test, CORE_makeSineWave) {
    // const std::array<double, 4> sine_4 = makeSineTable_debug<double, 4>(); 

    size_t idx=0;
    double error = 0.0001;
    double val;
    double expected;

    val = sine_4[idx];
    expected = expected_sine_4[idx];
    EXPECT_NEAR(expected, val, error);
    idx++;

    val = sine_4[idx];
    expected = expected_sine_4[idx];
    EXPECT_NEAR(expected, val, error);
    idx++;

    val = sine_4[idx];
    expected = expected_sine_4[idx];
    EXPECT_NEAR(expected, val, error);
    idx++;

    val = sine_4[idx];
    expected = expected_sine_4[idx];
    EXPECT_NEAR(expected, val, error);
    idx++;

}

using algae::dsp::core::oscillator::table_lookup_lin_interp;
using algae::dsp::core::oscillator::update_phase;

TEST(DSP_Test, CORE_lookupSine_linear_interp) {
    constexpr size_t TABLE_SIZE = 1024;
    const std::array<double, TABLE_SIZE> table = makeSineTable<double, TABLE_SIZE>(); 

    double error = 0.01;
    double val;
    double expected;
    double phase = 0;
    for(size_t i; i<48000; i++){
        val = table_lookup_lin_interp<double,TABLE_SIZE>(table.data(), phase);
        expected = sin(2*M_PI*phase);
        EXPECT_NEAR(expected, val, error);
        phase=update_phase<double,double>(phase,440.0/48000.0,1.0);
    }

}

using algae::dsp::core::oscillator::sine_t;

TEST(DSP_Test, CORE_sin_t) {
    size_t idx=0;
    double val;
    double expected;
    val = sine_t<double,4>::lookup(idx/4.0);
    expected = expected_sine_4[idx];
    EXPECT_FLOAT_EQ(expected, val);

}

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

using algae::dsp::core::oscillator::process_sineOsc;
TEST(DSP_Test, CORE_sine_t_process) {
 // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    constexpr float SR = 48000;


    AudioBlock<float,BLOCKSIZE> output;
    float phase=0;
    float phi = 2*M_PI/SR;

    std::tie(phase,output) = process_sineOsc<float, 1024, BLOCKSIZE>(phase, phi);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
}

using algae::dsp::core::oscillator::stk_blit_saw_t;
using algae::dsp::core::oscillator::stk_blit_saw;
using algae::dsp::core::oscillator::process;
TEST(DSP_Test, CORE_stk_blit_saw_process) {
 // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    constexpr float SR = 48000;


    stk_blit_saw_t<float> osc = stk_blit_saw<float,float>(440,SR);
    AudioBlock<float,BLOCKSIZE> output;


    std::tie(osc, output) = process<float, BLOCKSIZE>(osc);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
}

using algae::dsp::core::oscillator::stk_blit_square_t;
using algae::dsp::core::oscillator::stk_blit_square;
TEST(DSP_Test, CORE_stk_blit_square_process) {
 // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    constexpr float SR = 48000;


    stk_blit_square_t<float> osc = stk_blit_square<float,float>(440,SR);
    AudioBlock<float,BLOCKSIZE> output;


    std::tie(osc, output) = process<float, BLOCKSIZE>(osc);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
}
#include <gtest/gtest.h>
#include <iostream>
#include "../../src/core/audio_block.h"
#include "../../src/core/oscillator.h"

using algae::dsp::core::AudioBlock;

using algae::dsp::core::oscillator::makeCosTable;
const std::array<double, 4> sine_4 = makeCosTable<double, 4>(); 
const std::array<double, 4> expected_sine_4 = { 1, 0, -1, 0}; 
//sine wavetable
TEST(DSP_Test, CORE_makeCosWave) {

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

TEST(DSP_Test, CORE_lookup_cos_linear_interp) {
    constexpr size_t TABLE_SIZE = 1024;
    const std::array<double, TABLE_SIZE> table = makeCosTable<double, TABLE_SIZE>(); 

    double error = 0.01;
    double val;
    double expected;
    double phase = 0;
    for(size_t i; i<48000; i++){
        val = table_lookup_lin_interp<double,TABLE_SIZE>(table, phase);
        expected = cos(2*M_PI*phase);
        EXPECT_NEAR(expected, val, error);
        phase=update_phase<double,double>(phase,440.0/48000.0,1.0);
    }

}

using algae::dsp::core::oscillator::cos_t;

TEST(DSP_Test, CORE_cos_t) {
    size_t idx=0;
    double val;
    double expected;
    val = cos_t<double,4>::lookup(idx/4.0);
    expected = expected_sine_4[idx];
    EXPECT_FLOAT_EQ(expected, val);

}

TEST(DSP_Test, CORE_cos_t_template_specialized) {
    size_t idx=0;
    double val;
    double expected;
    val = cos_t<double,0>::lookup(idx/4.0);
    expected = cos(idx/4.0);
    EXPECT_FLOAT_EQ(expected, val);

}

#include <gtest/gtest.h>
#include <iostream>
#include "../../src/core/constants.h"
#include "../../src/core/audio_block.h"
#include "../../src/core/oscillator.h"

using algae::dsp::core::AudioBlock;


using algae::dsp::core::oscillator::compute_phase_increment;
TEST(Osc_Test, CORE_compute_phase_increment) {
    float sample_rate = 48000;
    float epsilon = 0.00000001;
    EXPECT_NEAR(0.000020833 ,compute_phase_increment<float>(1,sample_rate),epsilon);
    EXPECT_NEAR(0.00916667 ,compute_phase_increment<float>(440,sample_rate),epsilon);
    EXPECT_NEAR(0.052083333 ,compute_phase_increment<float>(2500,sample_rate),epsilon);
}

using algae::dsp::core::oscillator::update_phase;
TEST(Osc_Test, CORE_update_phasor) {
    float sample_rate = 48000;
    float period = algae::dsp::core::TWO_PI;
    float phi = compute_phase_increment<float>(4800,sample_rate);
    float phase = 0;
    float epsilon = 0.00001;

    std::array<float, 10> expected_phases = {0.628319,1.25664,1.88496,2.51327,3.14159,3.76991,4.39823,5.02655,5.65487};
    for(size_t frame_idx=0; frame_idx<10; frame_idx++){
        phase = update_phase(phase, phi, period);
        EXPECT_NEAR(expected_phases[frame_idx],phase,epsilon);
    }
    
  
}

using algae::dsp::core::simd::simd_traits;
using algae::dsp::core::simd::store_a;
using algae::dsp::core::simd::load_a;

TEST(Osc_Test, CORE_update_phasor__vector_float) {
    constexpr float epsilon = 0.0001;
    constexpr size_t num_cases = 16;
    typedef typename simd_traits<float>::type sample_vec;
    constexpr size_t vec_size = simd_traits<float>::size;
    sample_vec sample_rate = 48000;
    sample_vec period = algae::dsp::core::TWO_PI;
    sample_vec phi = compute_phase_increment<sample_vec>(sample_vec(4800),sample_rate);
    sample_vec phase = 0;
    alignas(vec_size) float actual[num_cases];

    std::array<float, num_cases> expected_phases 
        = {0.628319,1.25664,1.88496,2.51327,3.14159,3.76991,4.39823,5.02655,5.65487,0.00000469282041315466,0.628324692820413,1.25664469282041,1.88496469282041,2.51328469282041,3.14160469282041,3.76992469282041};
    for(size_t frame_idx=0; frame_idx<num_cases/vec_size; frame_idx++){
        phase = update_phase<sample_vec>(phase, phi, period);
        store_a(&actual[frame_idx*vec_size], phase);
    }

    for(size_t frame_idx=0; frame_idx<num_cases; frame_idx++)
        EXPECT_NEAR(expected_phases[frame_idx],actual[frame_idx],epsilon);
    
  
}


TEST(Osc_Test, CORE_update_phasor__vector_double) {
    constexpr double epsilon = 0.0001;
    constexpr size_t num_cases = 16;
    typedef typename simd_traits<double>::type sample_vec;
    constexpr size_t vec_size = simd_traits<double>::size;
    sample_vec sample_rate = 48000;
    sample_vec period = algae::dsp::core::TWO_PI;
    sample_vec phi = compute_phase_increment<sample_vec>(sample_vec(4800),sample_rate);
    sample_vec phase = 0;
    alignas(vec_size) double actual[num_cases];

    std::array<double, num_cases> expected_phases 
        = {0.628319,1.25664,1.88496,2.51327,3.14159,3.76991,4.39823,5.02655,5.65487,0.00000469282041315466,0.628324692820413,1.25664469282041,1.88496469282041,2.51328469282041,3.14160469282041,3.76992469282041};
    for(size_t frame_idx=0; frame_idx<num_cases/vec_size; frame_idx++){
        phase = update_phase<sample_vec>(phase, phi, period);
        store_a(&actual[frame_idx*vec_size], phase);
    }

    for(size_t frame_idx=0; frame_idx<num_cases; frame_idx++)
        EXPECT_NEAR(expected_phases[frame_idx],actual[frame_idx],epsilon);
    
  
}


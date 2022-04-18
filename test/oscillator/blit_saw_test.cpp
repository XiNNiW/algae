#include <gtest/gtest.h>
#include <iostream>

#include "../../src/core/audio_block.h"
#include "../../src/core/oscillator.h"
#include "../frequency_test_utilities.h"
#include "../../src/core/units/mtof.h"

#include "blit_saw_test.h"

using algae::dsp::core::AudioBlock;


using algae::dsp::core::oscillator::stk_blit_saw_t;
TEST(Osc_Test, CORE_stk_blit_struct_has_defaults) {

    stk_blit_saw_t<double> osc;
    
    EXPECT_EQ(0,osc.a);
    EXPECT_EQ(0,osc.c2);
    EXPECT_EQ(0,osc.lastFrame);
    EXPECT_EQ(0,osc.m);
    EXPECT_EQ(0,osc.p);
    EXPECT_EQ(0,osc.phase);
    EXPECT_EQ(0,osc.phase_increment);
    EXPECT_EQ(0,osc.state);
}

using algae::dsp::core::oscillator::stk_blit_saw;
TEST(Osc_Test, CORE_stk_blit_initializes_with_correct_values) {
    const double SR = 48000;

    stk_blit_saw_t<double> osc = stk_blit_saw<double,double>(480,SR);
    
    EXPECT_EQ(1.01,osc.a);
    EXPECT_EQ(0.01,osc.c2);
    EXPECT_EQ(0,osc.lastFrame);
    EXPECT_EQ(101,osc.m);
    EXPECT_EQ(100,osc.p);
    EXPECT_EQ(0.0,osc.phase);
    EXPECT_NEAR(0.0314159,osc.phase_increment,0.0001);
    EXPECT_EQ(-0.505,osc.state);
}

using algae::dsp::core::oscillator::process;
TEST(Osc_Test, CORE_stk_blit_saw_process_is_not_too_loud) {
    const size_t BLOCKSIZE = 64;
    const float SR = 48000;

    stk_blit_saw_t<float> osc = stk_blit_saw<float,float>(440,SR);
    AudioBlock<float,BLOCKSIZE> output;

    std::tie(osc, output) = process<float, BLOCKSIZE>(osc);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
}

using algae::dsp::core::oscillator::sineOsc;
using algae::dsp::core::units::mtof;
using algae::dsp::core::oscillator::compute_phase_increment;
TEST_P(STKBlitSawFixture, SpectraConformsToKnownSpectra){
    
    const auto [periodsPerFrame, expectedSpectra] = GetParam();
    // std::cout << "HARMONIC UNDER TEST: " << periodsPerBin << std::endl;

    const double expectedBaseFreq = periodsPerFrame/double(FREQ_BINS/SAMPLE_RATE);

    
    stk_blit_saw_t<double> osc = stk_blit_saw<double,double>(expectedBaseFreq,SAMPLE_RATE);
    AudioBlock<double,2*FREQ_BINS> sawtoothTimeseries;
    
    std::tie(osc, sawtoothTimeseries) = process<double, 2*FREQ_BINS>(osc);

    const std::array<double, FREQ_BINS> actualSpectra = compute_whole_spectrum_magnitude_in_db<double, FREQ_BINS>(sawtoothTimeseries, SAMPLE_RATE);

    const double epsilonInDB = 3;

    for(size_t bin=0; bin < FREQ_BINS; bin++){
        EXPECT_NEAR(expectedSpectra[bin], actualSpectra[bin], epsilonInDB);
        // std::cout<<actual_spectra[bin]<<",";

    }
    // std::cout<<std::endl;   
    // EXPECT_EQ(1,0); 
    
}


INSTANTIATE_TEST_CASE_P(
        SpectraConformsToKnownSpectra,
        STKBlitSawFixture,
        ::testing::Values(
                std::make_tuple(4.0, knownSawSpectra4PeriodsPerFrame),
                std::make_tuple(8.0, knownSawSpectra8PeriodsPerFrame),
                std::make_tuple(16.0, knownSawSpectra16PeriodsPerFrame)
        ));
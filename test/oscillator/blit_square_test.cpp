
#include <iostream>

#include "../../src/core/audio_block.h"
#include "../../src/core/oscillator.h"
#include "blit_square_test.h"
#include "../frequency_test_utilities.h"

using algae::dsp::core::AudioBlock;

using algae::dsp::core::oscillator::stk_blit_square_t;
TEST(DSP_Test, CORE_stk_blit_square_struct_has_defaults) {
    const size_t BLOCKSIZE = 64;
    const double SR = 48000;

    stk_blit_square_t<double> osc;
    
    EXPECT_EQ(0,osc.a);
    EXPECT_EQ(0,osc.dc_blocker_x1);
    EXPECT_EQ(0,osc.last_blit_output);
    EXPECT_EQ(0,osc.m);
    EXPECT_EQ(0,osc.p);
    EXPECT_EQ(0,osc.phase);
    EXPECT_EQ(0,osc.phase_increment);
    EXPECT_EQ(0,osc.state);
}

using algae::dsp::core::oscillator::stk_blit_square;
TEST(DSP_Test, CORE_stk_blit_square_initializes_with_correct_values) {
    const size_t BLOCKSIZE = 64;
    const double SR = 48000;

    stk_blit_square_t<double> osc = stk_blit_square<double,double>(480,SR);
    
    EXPECT_EQ(1.04,osc.a);
    EXPECT_EQ(0,osc.dc_blocker_x1);
    EXPECT_EQ(0,osc.last_blit_output);
    EXPECT_EQ(52,osc.m);
    EXPECT_EQ(50,osc.p);
    EXPECT_EQ(0.0,osc.phase);
    EXPECT_NEAR(0.062831853071795868,osc.phase_increment,0.0001);
    EXPECT_EQ(0,osc.state);
}
using algae::dsp::core::oscillator::process;
TEST(DSP_Test, CORE_stk_blit_square_process) {
 // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    constexpr float SR = 48000;


    stk_blit_square_t<float> osc = stk_blit_square<float,float>(440,SR);
    AudioBlock<float,BLOCKSIZE> output=AudioBlock<float,BLOCKSIZE>::empty();


    std::tie(osc, output) = process<float, BLOCKSIZE>(osc);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-10.1);
        EXPECT_LT(output[i],10.1);
    }
}

TEST_P(STKBlitSquareFixture, SpectraConformsToKnownSpectra){
    
    const auto [periodsPerFrame, expectedSpectra] = GetParam();
    // std::cout << "HARMONIC UNDER TEST: " << periodsPerBin << std::endl;

    const double expectedBaseFreq = periodsPerFrame/double(FREQ_BINS/SAMPLE_RATE);

    
    stk_blit_square_t<double> osc = stk_blit_square<double,double>(expectedBaseFreq,SAMPLE_RATE);
    AudioBlock<double,2*FREQ_BINS> sawtoothTimeseries;
    
    std::tie(osc, sawtoothTimeseries) = process<double, 2*FREQ_BINS>(osc);

    const std::array<double, FREQ_BINS> actualSpectra = compute_whole_spectrum_magnitude_in_db<double, FREQ_BINS>(sawtoothTimeseries, SAMPLE_RATE);

    const double epsilonInDB = 3;

    for(size_t bin=0; bin < FREQ_BINS; bin++){
        EXPECT_NEAR(expectedSpectra[bin], actualSpectra[bin], epsilonInDB);
        // std::cout<<actualSpectra[bin]<<",";

    }
    // std::cout<<std::endl;   
    // EXPECT_EQ(1,0); 
    
}


INSTANTIATE_TEST_CASE_P(
        SpectraConformsToKnownSpectra,
        STKBlitSquareFixture,
        ::testing::Values(
                std::make_tuple(4.0, knownSquareSpectra4PeriodsPerFrame),
                std::make_tuple(8.0, knownSquareSpectra8PeriodsPerFrame),
                std::make_tuple(16.0, knownSquareSpectra16PeriodsPerFrame)
        ));
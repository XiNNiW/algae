#include <gtest/gtest.h>
#include <iostream>

#include "../../src/core/audio_block.h"
#include "../../src/core/oscillator.h"
#include "../frequency_test_utilities.h"

using algae::dsp::core::AudioBlock;

using algae::dsp::core::oscillator::noise;
TEST(Osc_Test, CORE_noise_is_not_too_loud) {
    const size_t BLOCKSIZE = 64;

    auto nz = noise<float,BLOCKSIZE>();

    for(size_t i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(nz[i],-1.0001);
        EXPECT_LT(nz[i],1.0001);
    }
}

TEST(Osc_Test, NoiseSpectraContainsAllFrequencies){

    const size_t FREQ_BINS = 2*4096;
    const size_t SAMPLE_RATE = 48000;
    auto noiseTimeseries = noise<double,2*FREQ_BINS>();

    const std::array<double, FREQ_BINS> actualSpectra = compute_whole_spectrum_magnitude_in_db<double, FREQ_BINS>(noiseTimeseries, SAMPLE_RATE);

    const double epsilonInDB = 40;

    for(size_t bin=0; bin < FREQ_BINS; bin++){
        EXPECT_NEAR(0, actualSpectra[bin], epsilonInDB);
        // std::cout << noiseTimeseries[bin] << "," << actualSpectra[bin] << std::endl;

    }

    // EXPECT_EQ(1,0);

    
}

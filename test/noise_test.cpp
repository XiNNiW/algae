#include <array>
#include <gtest/gtest.h>
#include <iostream>

#include "algae.h"
#include "frequency_test_utilities.h"

using algae::dsp::oscillator::WhiteNoise;
TEST(Osc_Test, CORE_noise_is_not_too_loud) {
  const size_t BLOCKSIZE = 64;

  WhiteNoise noise = WhiteNoise<float>();
  float output[BLOCKSIZE];
  noise.process(BLOCKSIZE, output);

  for (size_t i = 0; i < BLOCKSIZE; i++) {
    EXPECT_GT(output[i], -1.0001);
    EXPECT_LT(output[i], 1.0001);
  }
}

TEST(Osc_Test, NoiseSpectraContainsAllFrequencies) {

  const size_t FREQ_BINS = 2 * 4096;
  const size_t BLOCKSIZE = 2 * FREQ_BINS;
  const size_t SAMPLE_RATE = 48000;
  WhiteNoise noise = WhiteNoise<double>();
  double noiseTimeseries[BLOCKSIZE];
  noise.process(BLOCKSIZE, noiseTimeseries);

  std::array<double, FREQ_BINS> actualSpectra =
      compute_whole_spectrum_magnitude_in_db<double, FREQ_BINS>(noiseTimeseries,
                                                                SAMPLE_RATE);

  const double epsilonInDB = 40;

  for (size_t bin = 0; bin < FREQ_BINS; bin++) {
    EXPECT_NEAR(0, actualSpectra[bin], epsilonInDB);
  }
}

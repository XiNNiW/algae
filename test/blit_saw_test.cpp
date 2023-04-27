#include <gtest/gtest.h>
#include <iostream>

#include "algae.h"
#include "blit_saw_test.h"
#include "frequency_test_utilities.h"

using algae::dsp::oscillator::STKBlitSaw;
// TEST(Osc_Test, CORE_stk_blit_struct_has_defaults) {
//
//   STKBlitSaw<double, double> osc = STKBlitSaw<double, double>(480, 48000);
//
//   EXPECT_EQ(0, osc.a);
//   EXPECT_EQ(0, osc.c2);
//   EXPECT_EQ(0, osc.lastFrame);
//   EXPECT_EQ(0, osc.m);
//   EXPECT_EQ(0, osc.p);
//   EXPECT_EQ(0, osc.phase);
//   EXPECT_EQ(0, osc.phase_increment);
//   EXPECT_EQ(0, osc.state);
// }

TEST(Osc_Test, CORE_stk_blit_initializes_with_correct_values) {
  const double SR = 48000;

  STKBlitSaw<double, double> osc = STKBlitSaw<double, double>(480, SR);
  osc.setFrequency(480, SR);

  EXPECT_EQ(1.01, osc.a);
  EXPECT_EQ(0.01, osc.c2);
  EXPECT_EQ(0, osc.lastFrame);
  EXPECT_EQ(101, osc.m);
  EXPECT_EQ(100, osc.p);
  EXPECT_EQ(0.0, osc.phase);
  EXPECT_NEAR(0.0314159, osc.phase_increment, 0.0001);
  EXPECT_EQ(-0.505, osc.state);
}

TEST(Osc_Test, CORE_stk_blit_saw_process_is_not_too_loud) {
  const size_t BLOCKSIZE = 64;
  const float SR = 48000;

  STKBlitSaw<float, float> osc = STKBlitSaw<float, float>(480, SR);
  float output[BLOCKSIZE];

  osc.process(BLOCKSIZE, output);

  for (size_t i = 0; i < BLOCKSIZE; i++) {
    EXPECT_GT(output[i], -1.0001);
    EXPECT_LT(output[i], 1.0001);
  }
}

TEST_P(STKBlitSawFixture, SpectraConformsToKnownSpectra) {
  const size_t BLOCKSIZE = 2 * FREQ_BINS;
  const auto [periodsPerFrame, expectedSpectra] = GetParam();
  // std::cout << "HARMONIC UNDER TEST: " << periodsPerBin << std::endl;

  const double expectedBaseFreq =
      periodsPerFrame / double(FREQ_BINS / SAMPLE_RATE);

  STKBlitSaw<double, double> osc =
      STKBlitSaw<double, double>(expectedBaseFreq, SAMPLE_RATE);

  double sawtoothTimeseries[BLOCKSIZE];
  osc.process(BLOCKSIZE, sawtoothTimeseries);

  std::array<double, FREQ_BINS> actualSpectra =
      compute_whole_spectrum_magnitude_in_db<double, FREQ_BINS>(
          sawtoothTimeseries, SAMPLE_RATE);

  const double epsilonInDB = 3;

  for (size_t bin = 0; bin < FREQ_BINS; bin++) {
    EXPECT_NEAR(expectedSpectra[bin], actualSpectra[bin], epsilonInDB);
    // std::cout<<actual_spectra[bin]<<",";
  }
  // std::cout<<std::endl;
  // EXPECT_EQ(1,0);
}

INSTANTIATE_TEST_CASE_P(
    SpectraConformsToKnownSpectra, STKBlitSawFixture,
    ::testing::Values(std::make_tuple(4.0, knownSawSpectra4PeriodsPerFrame),
                      std::make_tuple(8.0, knownSawSpectra8PeriodsPerFrame),
                      std::make_tuple(16.0, knownSawSpectra16PeriodsPerFrame)));

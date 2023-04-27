#include "blit_square_test.h"
#include "algae.h"
#include "frequency_test_utilities.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::oscillator::STKBlitSquare;
TEST(Osc_Test, CORE_stk_blit_square_struct_has_defaults) {

  STKBlitSquare<double, double> osc;

  EXPECT_EQ(0, osc.a);
  EXPECT_EQ(0, osc.dc_blocker_x1);
  EXPECT_EQ(0, osc.last_blit_output);
  EXPECT_EQ(0, osc.m);
  EXPECT_EQ(0, osc.p);
  EXPECT_EQ(0, osc.phase);
  EXPECT_EQ(0, osc.phase_increment);
  EXPECT_EQ(0, osc.state);
}

TEST(Osc_Test, CORE_stk_blit_square_initializes_with_correct_values) {
  const double SR = 48000;

  STKBlitSquare<double, double> osc;
  osc.setFrequency(480, SR);

  EXPECT_EQ(1.04, osc.a);
  EXPECT_EQ(0, osc.dc_blocker_x1);
  EXPECT_EQ(0, osc.last_blit_output);
  EXPECT_EQ(52, osc.m);
  EXPECT_EQ(50, osc.p);
  EXPECT_EQ(0.0, osc.phase);
  EXPECT_NEAR(0.062831853071795868, osc.phase_increment, 0.0001);
  EXPECT_EQ(0, osc.state);
}
TEST(Osc_Test, CORE_stk_blit_square_process) {
  constexpr size_t BLOCKSIZE = 64;
  constexpr float SR = 48000;

  STKBlitSquare<double, double> osc;
  osc.setFrequency(480, SR);

  double output[BLOCKSIZE];
  osc.process(BLOCKSIZE, output);

  for (size_t i = 0; i < BLOCKSIZE; i++) {
    EXPECT_GT(output[i], -10.1);
    EXPECT_LT(output[i], 10.1);
  }
}

TEST_P(STKBlitSquareFixture, SpectraConformsToKnownSpectra) {

  const auto [periodsPerFrame, expectedSpectra] = GetParam();
  // std::cout << "HARMONIC UNDER TEST: " << periodsPerBin << std::endl;

  const double expectedBaseFreq =
      periodsPerFrame / double(FREQ_BINS / SAMPLE_RATE);

  STKBlitSquare<double, double> osc;
  osc.setFrequency(expectedBaseFreq, SAMPLE_RATE);

  double sawtoothTimeseries[2 * FREQ_BINS];

  osc.process(2 * FREQ_BINS, sawtoothTimeseries);

  std::array<double, FREQ_BINS> actualSpectra =
      compute_whole_spectrum_magnitude_in_db<double, FREQ_BINS>(
          sawtoothTimeseries, SAMPLE_RATE);

  const double epsilonInDB = 3;

  for (size_t bin = 0; bin < FREQ_BINS; bin++) {
    EXPECT_NEAR(expectedSpectra[bin], actualSpectra[bin], epsilonInDB);
    // std::cout<<actualSpectra[bin]<<",";
  }
  // std::cout<<std::endl;
  // EXPECT_EQ(1,0);
}

INSTANTIATE_TEST_CASE_P(
    SpectraConformsToKnownSpectra, STKBlitSquareFixture,
    ::testing::Values(std::make_tuple(4.0, knownSquareSpectra4PeriodsPerFrame),
                      std::make_tuple(8.0, knownSquareSpectra8PeriodsPerFrame),
                      std::make_tuple(16.0,
                                      knownSquareSpectra16PeriodsPerFrame)));

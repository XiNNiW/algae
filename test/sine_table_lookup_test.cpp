#include "sine_table_lookup_test.h"
#include "algae.h"
#include "frequency_test_utilities.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::oscillator::makeSineTable;
const std::array<double, 4> sine_4 = makeSineTable<double, 4>();
const std::array<double, 4> expected_sine_4 = {0, 1, 0, -1};
// sine wavetable
TEST(DSP_Test, CORE_makeSineWave) {

  size_t idx = 0;
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

using algae::dsp::oscillator::table_lookup_linear_interp;
using algae::dsp::oscillator::update_phase;

TEST(DSP_Test, CORE_lookupSine_linear_interp) {
  constexpr size_t TABLE_SIZE = 1024;
  const std::array<double, TABLE_SIZE> table =
      makeSineTable<double, TABLE_SIZE>();

  double error = 0.01;
  double val;
  double expected;
  double phase = 0;
  for (size_t i = 0; i < 48000; i++) {
    val = table_lookup_linear_interp<double>(table.data(), TABLE_SIZE, phase);
    expected = sin(2 * M_PI * phase);
    EXPECT_NEAR(expected, val, error);
    phase = update_phase<double>(phase, 440.0 / 48000.0, 1.0);
  }
}

using algae::dsp::oscillator::SineTable;

TEST(DSP_Test, CORE_sin_t) {
  size_t idx = 0;
  double val;
  double expected;
  val = SineTable<double, 4>::lookup(idx / 4.0);
  expected = expected_sine_4[idx];
  EXPECT_FLOAT_EQ(expected, val);
}

TEST(DSP_Test, CORE_sin_t_template_specialized) {
  size_t idx = 0;
  double val;
  double expected;
  val = SineTable<double, 0>::lookup(idx / 4.0);
  expected = sin(idx / 4.0);
  EXPECT_FLOAT_EQ(expected, val);
}

using algae::dsp::oscillator::SinOscillator;
TEST(DSP_Test, CORE_sine_t_process) {
  constexpr size_t BLOCKSIZE = 64;
  constexpr float SR = 48000;

  double output[BLOCKSIZE];
  // float phi = 2 * M_PI / SR;

  SinOscillator<double, double> sinOsc;

  sinOsc.setFrequency(1, SR);

  sinOsc.process(BLOCKSIZE, output);

  for (size_t i = 0; i < BLOCKSIZE; i++) {
    EXPECT_GT(output[i], -1.0001);
    EXPECT_LT(output[i], 1.0001);
  }
}

TEST_P(SineOscFixture, SpectraConformsToKnownSpectra) {
  const size_t BLOCKSIZE = 2 * FREQ_BINS;
  SinOscillator<double, double> sinOsc;
  const auto periodsInFrame = GetParam();
  // std::cout << "HARMONIC UNDER TEST: " << periodsPerBin << std::endl;

  const double expectedBaseFreq =
      periodsInFrame / double(FREQ_BINS / SAMPLE_RATE);
  const size_t bin_of_fundamental_freq = expectedBaseFreq / resolution;
  sinOsc.setFrequency(expectedBaseFreq, SAMPLE_RATE);

  double sineTimeseries[BLOCKSIZE];
  sinOsc.process(BLOCKSIZE, sineTimeseries);
  std::array<double, FREQ_BINS> actualSpectra =
      compute_whole_spectrum_magnitude_in_db<double, FREQ_BINS>(sineTimeseries,
                                                                SAMPLE_RATE);

  const double quietInDB = -27;

  for (size_t bin = 0; bin < bin_of_fundamental_freq; bin++) {
    EXPECT_LT(actualSpectra[bin], quietInDB);
  }

  EXPECT_NEAR(0, actualSpectra[bin_of_fundamental_freq], 3);

  for (size_t bin = bin_of_fundamental_freq + 1; bin < FREQ_BINS; bin++) {
    EXPECT_LT(actualSpectra[bin], quietInDB);
  }
  // std::cout<<std::endl;
  // EXPECT_EQ(1,0);
}

INSTANTIATE_TEST_CASE_P(SpectraConformsToKnownSpectra, SineOscFixture,
                        ::testing::Values(4, 8, 16));

#include "algae.h"
#include "frequency_test_utilities.h"
#include <gtest/gtest.h>

using algae::dsp::filter::Allpass2Comb;

TEST(Filter_Test, CORE_allpass_2comb_construction) {
  constexpr size_t SIZE = 4800;
  const double g = 0.943;
  const double delay_time_samps = 112;
  Allpass2Comb<double, SIZE> filter;
  filter.set(delay_time_samps, g);

  EXPECT_EQ(g, filter.g);
  EXPECT_EQ(0, filter.write_index);
  for (size_t idx = 0; idx < SIZE - 1; idx++) {
    EXPECT_EQ(0, filter.xn[idx]);
    EXPECT_EQ(0, filter.yn[idx]);
  }
}

TEST(Filter_Test, CORE_allpass_2comb_does_not_explode) {
  constexpr size_t BLOCKSIZE = 64;
  constexpr size_t SIZE = 4800;
  const double g = 0.943;
  const double delay_time_samps = 112;
  Allpass2Comb<double, SIZE> filter;
  filter.set(delay_time_samps, g);

  double output[BLOCKSIZE];
  double noiseBuffer[BLOCKSIZE];
  algae::dsp::oscillator::WhiteNoise<double> noiseSource;
  noiseSource.process(BLOCKSIZE, noiseBuffer);
  filter.process(noiseBuffer, BLOCKSIZE, output);

  double max_magnitude = 0;
  for (size_t i = 0; i < BLOCKSIZE; i++) {
    // std::cout << "output: " << output[i] << ", input: " << input[i] <<"\n";
    max_magnitude =
        abs(output[i]) > max_magnitude ? abs(output[i]) : max_magnitude;
  }

  EXPECT_LT(max_magnitude, 5);
  EXPECT_GT(max_magnitude, 0);
}

TEST(Filter_Test, CORE_allpass_2comb_amplitude_response) {
  constexpr size_t NUM_BINS = 32;
  constexpr double SAMPLE_RATE = 192000;
  constexpr size_t SIZE = 4800;
  const double g = 0.943;
  const double delay_time_samps = 112;
  Allpass2Comb<double, SIZE> filter;
  filter.set(delay_time_samps, g);

  std::array<double, NUM_BINS> amp_response =
      compute_amplitude_response<double, Allpass2Comb<double, SIZE>, NUM_BINS>(
          filter, SAMPLE_RATE);

  // expected values computed in signal python using freqz function to evaluate
  // filter response based on coefficients
  std::array<double, NUM_BINS> expected_response = {1.0,
                                                    1.000000000000001,
                                                    1.0000000000000002,
                                                    0.9999999999999998,
                                                    0.9999999999999997,
                                                    0.9999999999999999,
                                                    0.9999999999999999,
                                                    0.9999999999999992,
                                                    1.0,
                                                    0.9999999999999991,
                                                    1.0000000000000018,
                                                    1.0000000000000007,
                                                    1.0000000000000007,
                                                    1.0000000000000082,
                                                    0.9999999999999991,
                                                    1.0,
                                                    1.0,
                                                    1.0000000000000453,
                                                    0.9999999999999987,
                                                    0.9999999999999999,
                                                    1.0000000000000002,
                                                    1.0000000000000009,
                                                    1.0000000000000004,
                                                    1.0000000000000013,
                                                    1.0000000000000004,
                                                    1.0000000000000002,
                                                    1.0000000000000002,
                                                    1.0000000000000004,
                                                    1.000000000000004,
                                                    0.9999999999999991,
                                                    1.0000000000000004,
                                                    0.9999999999999998

  };
  for (size_t bin_idx = 0; bin_idx < NUM_BINS; bin_idx++) {
    EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx], 0.06);
  }
}

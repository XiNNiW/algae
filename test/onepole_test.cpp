#include "algae.h"
#include "frequency_test_utilities.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::filter::Onepole;
using algae::dsp::oscillator::WhiteNoise;

TEST(Filter_Test, CORE_onepole_lop_defaults) {
  Onepole<double, double> filter;
  EXPECT_EQ(0, filter.a0);
  EXPECT_EQ(0, filter.y1);
  EXPECT_EQ(1, filter.type_coefficient);
}

TEST(Filter_Test, CORE_onepole_lop_initializes_coefficients) {
  Onepole<double, double> filter;
  filter.lowpass(880.0, 48000.0);

  EXPECT_NEAR(0.057564027, filter.a0, 0.001);
  EXPECT_EQ(1, filter.type_coefficient);
}

TEST(Filter_Test, CORE_onepole_lop_does_not_explode) {
  const size_t BLOCKSIZE = 64;
  Onepole<double, double> filter;
  filter.lowpass(880.0, 48000.0);
  double input[BLOCKSIZE];
  double output[BLOCKSIZE];

  filter.lowpass(220.0, 48000.0);

  WhiteNoise<double> noiseGen;
  noiseGen.process(BLOCKSIZE, input);

  filter.process(input, BLOCKSIZE, output);

  for (size_t i = 0; i < BLOCKSIZE; i++) {
    EXPECT_GT(output[i], -1.0001);
    EXPECT_LT(output[i], 1.0001);
  }
}

TEST(Filter_Test, CORE_onepole_lop_amplitude_response) {
  constexpr size_t NUM_BINS = 32;
  constexpr double SAMPLE_RATE = 48000;

  Onepole<double, double> filter;
  filter.lowpass(800.0, 48000.0);

  std::cout << "a0 = " << filter.a0 << std::endl;

  std::array<double, NUM_BINS> amp_response =
      compute_amplitude_response<double, Onepole<double, double>, NUM_BINS>(
          filter, SAMPLE_RATE);

  // expected values computed in octave using freqz function to evaluate filter
  // response based on coefficients
  std::array<double, NUM_BINS> expected_response = {
      0.967946,  0.465058,  0.256003,  0.174424,  0.132122,  0.106434,
      0.0892516, 0.0769889, 0.0678244, 0.0607359, 0.0551065, 0.0505419,
      0.0467786, 0.043634,  0.0409776, 0.0387135, 0.0367702, 0.0350929,
      0.0336393, 0.032376,  0.0312767, 0.0303201, 0.0294892, 0.0287699,
      0.0281511, 0.0276234, 0.0271793, 0.0268128, 0.026519,  0.0262942,
      0.0261356, 0.0260412};
  for (size_t bin_idx = 0; bin_idx < NUM_BINS; bin_idx++) {
    EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx], 0.01);
    // std::cout << amp_response[bin_idx] << std::endl;
  }
  // EXPECT_EQ(1,0);
}

TEST(Filter_Test, CORE_onepole_hip_initializes_coefficients) {
  Onepole<double, double> filter;
  filter.highpass(880.0, 48000.0);

  EXPECT_NEAR(0.057564027, filter.a0, 0.001);
  EXPECT_EQ(-1, filter.type_coefficient);
}

TEST(Filter_Test, CORE_onepole_hip_does_not_explode) {
  const size_t BLOCKSIZE = 64;
  Onepole<double, double> filter;
  filter.highpass(220.0, 48000.0);
  double input[BLOCKSIZE];
  double output[BLOCKSIZE];

  WhiteNoise<double> noiseGen;
  noiseGen.process(BLOCKSIZE, input);

  filter.process(input, BLOCKSIZE, output);

  for (size_t i = 0; i < BLOCKSIZE; i++) {
    EXPECT_GT(output[i], -1.0001);
    EXPECT_LT(output[i], 1.0001);
  }
}

TEST(Filter_Test, CORE_onepole_hip_amplitude_response) {
  constexpr size_t NUM_BINS = 32;
  constexpr double SAMPLE_RATE = 48000;

  Onepole<double, double> filter;
  filter.highpass(800.0, SAMPLE_RATE);

  std::cout << "a0 = " << filter.a0 << std::endl;

  std::array<double, NUM_BINS> amp_response =
      compute_amplitude_response<double, Onepole<double, double>, NUM_BINS>(
          filter, SAMPLE_RATE);

  // expected values computed in octave using freqz function to evaluate filter
  // response based on coefficients
  std::array<double, NUM_BINS> expected_response = {
      0.0260098, 0.0260412, 0.0261356, 0.0262942, 0.026519,  0.0268128,
      0.0271793, 0.0276234, 0.0281511, 0.0287699, 0.0294892, 0.0303201,
      0.0312767, 0.032376,  0.0336393, 0.0350929, 0.0367702, 0.0387135,
      0.0409776, 0.043634,  0.0467786, 0.0505419, 0.0551065, 0.0607359,
      0.0678244, 0.0769889, 0.0892516, 0.106434,  0.132122,  0.174424,
      0.256003,  0.465058};
  for (size_t bin_idx = 0; bin_idx < NUM_BINS; bin_idx++) {
    EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx], 0.01);
    // std::cout << amp_response[bin_idx] << std::endl;
  }
  // EXPECT_EQ(1,0);
}

using algae::dsp::filter::OnepoleOnezero;
TEST(Filter_Test, CORE_onepole_onezero_hip_defaults) {
  OnepoleOnezero<double, double> filter;
  EXPECT_EQ(0, filter.a1);
  EXPECT_EQ(0, filter.b0);
  EXPECT_EQ(0, filter.x1);
  EXPECT_EQ(0, filter.y1);
}

TEST(Filter_Test, CORE_onepole_onezero_hip_initializes_coefficients) {
  OnepoleOnezero<double, double> filter;
  filter.highpass(440.0, 48000.0);

  EXPECT_NEAR(0.944001, filter.a1, 0.001);
  EXPECT_NEAR(0.972001, filter.b0, 0.001);
}

TEST(Filter_Test, CORE_onepole_onezero_hip_does_not_explode) {
  const size_t BLOCKSIZE = 64;
  OnepoleOnezero<double, double> filter;
  filter.highpass(220.0, 48000.0);

  double input[BLOCKSIZE];
  double output[BLOCKSIZE];

  WhiteNoise<double> noiseGen;
  noiseGen.process(BLOCKSIZE, input);

  filter.process(input, BLOCKSIZE, output);

  for (size_t i = 0; i < BLOCKSIZE; i++) {
    EXPECT_GT(output[i], -2);
    EXPECT_LT(output[i], 2);
  }
}

TEST(Filter_Test, CORE_onepole_onezero_hip_amplitude_response) {
  constexpr size_t NUM_BINS = 32;
  constexpr double SAMPLE_RATE = 48000;

  OnepoleOnezero<double, double> filter;
  filter.highpass(80, 48000.0);
  std::cout << "a1 = " << filter.a1 << std::endl;
  std::cout << "b0 = " << filter.b0 << std::endl;

  std::array<double, NUM_BINS> amp_response =
      compute_amplitude_response<double, OnepoleOnezero<double, double>,
                                 NUM_BINS>(filter, SAMPLE_RATE);

  // expected values computed in octave using freqz function to evaluate filter
  // response based on coefficients
  std::array<double, NUM_BINS> expected_response = {
      0.51429, 0.998534, 1.00165, 1.00223, 1.00244, 1.00253, 1.00258, 1.00261,
      1.00263, 1.00265,  1.00266, 1.00266, 1.00267, 1.00267, 1.00268, 1.00268,
      1.00268, 1.00268,  1.00269, 1.00269, 1.00269, 1.00269, 1.00269, 1.00269,
      1.00269, 1.00269,  1.00269, 1.00269, 1.00269, 1.00269, 1.00269, 1.00269};
  for (size_t bin_idx = 0; bin_idx < NUM_BINS; bin_idx++) {
    EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx], 0.01);
    // std::cout << amp_response[bin_idx] << std::endl;
  }
  // EXPECT_EQ(1,0);
}

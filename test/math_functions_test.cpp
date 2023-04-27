#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::math::tanh_approx_pade;
using algae::dsp::oscillator::WhiteNoise;
TEST(DSP_Test, block_tanh_approximation_clamps_signal) {
  const auto BLOCKSIZE = 64;

  double input[BLOCKSIZE];
  double output[BLOCKSIZE];
  WhiteNoise<double> noiseGen;
  noiseGen.process(BLOCKSIZE, input);

  algae::dsp::block::mult<double>(input, 4, BLOCKSIZE, input);

  tanh_approx_pade<double>(input, BLOCKSIZE, output);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    EXPECT_GT(output[idx], -1.01);
    EXPECT_LT(output[idx], 1.01);
  }
}

TEST(DSP_Test, block_tanh_approximation) {
  const auto BLOCKSIZE = 64;
  float input[BLOCKSIZE];
  float expected[BLOCKSIZE];
  float actual[BLOCKSIZE];
  WhiteNoise<float> noiseGen;
  noiseGen.process(BLOCKSIZE, input);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    expected[idx] = tanh(input[idx]);
  }
  tanh_approx_pade<float>(input, BLOCKSIZE, actual);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    EXPECT_NEAR(expected[idx], actual[idx], 0.1);
  }
}

TEST(DSP_Test, block_atan_op) {
  const auto BLOCKSIZE = 64;
  float input[BLOCKSIZE];
  float expected[BLOCKSIZE];
  float actual[BLOCKSIZE];
  WhiteNoise<float> noiseGen;
  noiseGen.process(BLOCKSIZE, input);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    expected[idx] = atan(input[idx]);
  }

  algae::dsp::math::atan<float>(input, BLOCKSIZE, actual);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    EXPECT_NEAR(expected[idx], actual[idx], 0.001);
  }
}

TEST(DSP_Test, block_clip_op) {
  const auto BLOCKSIZE = 4;

  float input[BLOCKSIZE] = {0.1, -0.4, 1.1, -45};
  float expected[BLOCKSIZE] = {0.1, -0.4, 1.0, -1.0};
  float actual[BLOCKSIZE];

  algae::dsp::math::clip<float>(input, BLOCKSIZE, actual);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    EXPECT_NEAR(expected[idx], actual[idx], 0.001);
  }
}

TEST(DSP_Test, block_wrap_op) {
  const auto BLOCKSIZE = 4;

  float input[BLOCKSIZE] = {0.1, -0.4, 1.1, -1.1};
  float expected[BLOCKSIZE] = {0.1, 0.6, 0.1, 0.9};
  float actual[BLOCKSIZE];
  algae::dsp::math::wrap<float>(input, BLOCKSIZE, actual);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    EXPECT_NEAR(expected[idx], actual[idx], 0.001);
  }
}

TEST(DSP_Test, block_clamp_op) {
  const auto BLOCKSIZE = 4;

  float input[BLOCKSIZE] = {0.1, -0.4, 1.1, -1.1};
  float low[BLOCKSIZE] = {-0.5, -0.5, -0.5, -0.5};
  float high[BLOCKSIZE] = {0.5, 0.5, 0.5, 0.5};
  float expected[BLOCKSIZE] = {0.1, -0.4, 0.5, -0.5};
  float actual[BLOCKSIZE];
  algae::dsp::math::clamp<float>(input, low, high, BLOCKSIZE, actual);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    EXPECT_NEAR(expected[idx], actual[idx], 0.001);
  }
}

// TEST(DSP_Test, block_wrapto_op) {
//   const auto BLOCKSIZE = 4;
//
//   float input[BLOCKSIZE] = {0.1, -0.4, 1.1, -1.1};
//   float wrapPoint[BLOCKSIZE] = {-0.5, -0.5, -0.5, -0.5};
//   float expected[BLOCKSIZE] = {-0.05, -0.3, -0.05, -0.449};
//   float actual[BLOCKSIZE];
//
//   algae::dsp::math::wrapto<float>(input, wrapPoint, BLOCKSIZE, actual);
//
//   for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
//     EXPECT_NEAR(expected[idx], actual[idx], 0.001);
//   }
// }

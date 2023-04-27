#include "algae.h"
#include <gtest/gtest.h>

TEST(DSP_Test, Core_block_empty) {

  constexpr auto BLOCKSIZE = 64;
  float x[BLOCKSIZE];

  algae::dsp::block::empty<float>(BLOCKSIZE, x);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    ASSERT_EQ(x[idx], 0);
  }
}

TEST(DSP_Test, Core_block_multiply) {

  constexpr auto BLOCKSIZE = 64;

  float x[BLOCKSIZE];
  float y[BLOCKSIZE];
  float output[BLOCKSIZE];

  algae::dsp::block::fill<float>(2, BLOCKSIZE, x);
  algae::dsp::block::fill<float>(3, BLOCKSIZE, y);

  algae::dsp::block::mult<float>(x, y, BLOCKSIZE, output);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    EXPECT_FLOAT_EQ(6, output[idx]);
  }
}

TEST(DSP_Test, Core_block_divide) {

  constexpr auto BLOCKSIZE = 64;

  float x[BLOCKSIZE];
  float y[BLOCKSIZE];
  float output[BLOCKSIZE];

  algae::dsp::block::fill<float>(9, BLOCKSIZE, x);
  algae::dsp::block::fill<float>(3, BLOCKSIZE, y);

  algae::dsp::block::div<float>(x, y, BLOCKSIZE, output);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    EXPECT_FLOAT_EQ(3, output[idx]);
  }
}

TEST(DSP_Test, Core_block_add) {

  constexpr auto BLOCKSIZE = 64;

  float x[BLOCKSIZE];
  float y[BLOCKSIZE];
  float output[BLOCKSIZE];

  algae::dsp::block::fill<float>(9, BLOCKSIZE, x);
  algae::dsp::block::fill<float>(3, BLOCKSIZE, y);

  algae::dsp::block::add<float>(x, y, BLOCKSIZE, output);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    EXPECT_FLOAT_EQ(12, output[idx]);
  }
}

TEST(DSP_Test, Core_block_subtract) {

  constexpr auto BLOCKSIZE = 64;

  float x[BLOCKSIZE];
  float y[BLOCKSIZE];
  float output[BLOCKSIZE];

  algae::dsp::block::fill<float>(9, BLOCKSIZE, x);
  algae::dsp::block::fill<float>(3, BLOCKSIZE, y);

  algae::dsp::block::sub<float>(x, y, BLOCKSIZE, output);

  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    EXPECT_FLOAT_EQ(6, output[idx]);
  }
}

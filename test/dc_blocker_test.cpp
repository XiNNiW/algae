#include "algae.h"
#include "frequency_test_utilities.h"
#include <gtest/gtest.h>
#include <math.h>

using algae::dsp::filter::DCBlocker;
TEST(Filter_Test, CORE_DC_blocker_blocks_DC_offset) {

  const size_t BLOCKSIZE = 1000;

  DCBlocker<double> filter;
  const double DCoffset = 100;
  double input[BLOCKSIZE];
  double output[BLOCKSIZE];

  algae::dsp::oscillator::SinOscillator<double, double> osc;
  osc.process(BLOCKSIZE, input);

  algae::dsp::block::add(DCoffset, input, BLOCKSIZE, input);

  filter.process(input, BLOCKSIZE, output);

  EXPECT_LT(filter.y1, 2);
}

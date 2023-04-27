#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::math::msToSamples;

TEST(DSP_Test, CORE_mstosamps) {

  double ms = 1000.0;
  double sample_rate = 44100;

  ASSERT_FLOAT_EQ(44100.0, msToSamples(ms, sample_rate));

  ms = 500.0;
  sample_rate = 44100;

  ASSERT_FLOAT_EQ(22050.0, msToSamples(ms, sample_rate));

  ms = 1000;
  sample_rate = 8000.0;

  ASSERT_FLOAT_EQ(8000.0, msToSamples(ms, sample_rate));
}

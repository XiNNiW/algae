
#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::math::beatsToSamples;
TEST(DSP_Test, CORE_beats) {

  double bpm = 120;
  double num_beats = 4;

  ASSERT_FLOAT_EQ(96000, beatsToSamples<double>(num_beats, bpm, 48000));
}

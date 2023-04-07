#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::control::Ramp;

TEST(DSP_Test, CoreRampTest) {

  int ramptimeInSamples = 4;
  double initialValue = 0;
  double finalValue = 1;
  Ramp<double> envState;

  envState.set(initialValue, finalValue, ramptimeInSamples);
  envState.trigger();
  double out[1] = {0};
  envState.process(1, out);
  EXPECT_FLOAT_EQ(0, out[0]);

  envState.process(1, out);
  EXPECT_FLOAT_EQ(0.25, out[0]);

  envState.process(1, out);
  EXPECT_FLOAT_EQ(0.5, out[0]);

  envState.process(1, out);
  EXPECT_FLOAT_EQ(0.75, out[0]);

  envState.process(1, out);
  EXPECT_FLOAT_EQ(1, out[0]);

  envState.process(1, out);
  EXPECT_FLOAT_EQ(1, out[0]);
}

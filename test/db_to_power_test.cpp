#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::math::dbtopow;
TEST(DSP_Test, CORE_dbtopow) {

  double db = 100;
  double power = 1;

  ASSERT_FLOAT_EQ(power, dbtopow<double>(db));
}

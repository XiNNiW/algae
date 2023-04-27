#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::math::dbtorms;
TEST(DSP_Test, CORE_dbtorms) {

  double db = 100;
  double rms = 1;

  ASSERT_FLOAT_EQ(rms, dbtorms<double>(db));
}

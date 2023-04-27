#include <gtest/gtest.h>
#include <iostream>

#include "algae.h"
#include "frequency_test_utilities.h"

using algae::dsp::oscillator::blep;
TEST(Osc_Test, CORE_blep) {
  auto epsilon = 0.001;
  EXPECT_NEAR(-0.25, blep(1.0, 2.0), epsilon);
  EXPECT_NEAR(-0.5708, blep(1.1, 4.5), epsilon);
  EXPECT_NEAR(4, blep(2.0, 1.0), epsilon);
  EXPECT_NEAR(9.5537, blep(3.3, 1.1), epsilon);
  EXPECT_NEAR(1, blep(1.0, 1.0), epsilon);
  EXPECT_NEAR(0, blep(0.0, 0.0), epsilon);
}

using algae::dsp::oscillator::blamp;
TEST(Osc_Test, CORE_blamp) {
  auto epsilon = 0.001;
  EXPECT_NEAR(0.041666, blamp(1.0, 2.0), epsilon);
  EXPECT_NEAR(0.143773, blamp(1.1, 4.5), epsilon);
  EXPECT_NEAR(2.666666, blamp(2.0, 1.0), epsilon);
  EXPECT_NEAR(9.843225, blamp(3.3, 1.1), epsilon);
  EXPECT_NEAR(0.333333, blamp(1.0, 1.0), epsilon);
  EXPECT_NEAR(0, blamp(0.0, 0.0), epsilon);
}

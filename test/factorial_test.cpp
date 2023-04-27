#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::math::factorial_t;
TEST(Osc_Test, CORE_factorial_t) {
  double expected;
  double actual;

  expected = 1;
  actual = factorial_t<double, 1>::result;
  EXPECT_FLOAT_EQ(expected, actual);

  expected = 2;
  actual = factorial_t<double, 2>::result;
  EXPECT_FLOAT_EQ(expected, actual);

  expected = 3 * 2 * 1;
  actual = factorial_t<double, 3>::result;
  EXPECT_FLOAT_EQ(expected, actual);

  expected = 4 * 3 * 2 * 1;
  actual = factorial_t<double, 4>::result;
  EXPECT_FLOAT_EQ(expected, actual);
}

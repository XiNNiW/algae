#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::control::blackman_harris;
TEST(Control_Test, CoreWindowTest_BlackmanHarris) {
  size_t N = 10;
  size_t idx = 0;

  ASSERT_FLOAT_EQ(0.000059999998,
                  blackman_harris<double>(double(idx++) / double(N)));
  ASSERT_FLOAT_EQ(0.010982331,
                  blackman_harris<double>(double(idx++) / double(N)));
  ASSERT_FLOAT_EQ(0.10301149,
                  blackman_harris<double>(double(idx++) / double(N)));
  ASSERT_FLOAT_EQ(0.38589266,
                  blackman_harris<double>(double(idx++) / double(N)));
  ASSERT_FLOAT_EQ(0.79383349,
                  blackman_harris<double>(double(idx++) / double(N)));
  ASSERT_FLOAT_EQ(1, blackman_harris<double>(double(idx++) / double(N)));
  ASSERT_FLOAT_EQ(0.79383349,
                  blackman_harris<double>(double(idx++) / double(N)));
  ASSERT_FLOAT_EQ(0.38589266,
                  blackman_harris<double>(double(idx++) / double(N)));
  ASSERT_FLOAT_EQ(0.10301149,
                  blackman_harris<double>(double(idx++) / double(N)));
  ASSERT_FLOAT_EQ(0.010982331,
                  blackman_harris<double>(double(idx++) / double(N)));
  ASSERT_FLOAT_EQ(0.000059999998,
                  blackman_harris<double>(double(idx++) / double(N)));
}

using algae::dsp::block::add;
using algae::dsp::control::apply_window;

TEST(Control_Test, CoreWindowTest_window_variable_block) {
  constexpr size_t N = 10;
  double block[N] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  add<double>(block, 1, N, block);

  apply_window<double, blackman_harris>(block, N, block);
  for (size_t i = 0; i < N; i++) {
    std::cout << block[i] << "\n";
  }

  size_t idx = 0;
  ASSERT_FLOAT_EQ(0.000059999998, block[idx++]);
  ASSERT_FLOAT_EQ(0.010982331, block[idx++]);
  ASSERT_FLOAT_EQ(0.10301149, block[idx++]);
  ASSERT_FLOAT_EQ(0.38589266, block[idx++]);
  ASSERT_FLOAT_EQ(0.79383349, block[idx++]);
  ASSERT_FLOAT_EQ(1, block[idx++]);
  ASSERT_FLOAT_EQ(0.79383349, block[idx++]);
  ASSERT_FLOAT_EQ(0.38589266, block[idx++]);
  ASSERT_FLOAT_EQ(0.10301149, block[idx++]);
  ASSERT_FLOAT_EQ(0.010982331, block[idx++]);
}

TEST(Control_Test, CoreWindowTest_window_block) {
  constexpr size_t N = 10;
  double block[N];
  algae::dsp::block::empty(N, block);
  algae::dsp::block::add<double>(block, 1, N, block);

  apply_window<double, blackman_harris>(block, N, block);
  for (size_t i = 0; i < N; i++) {
    std::cout << block[i] << "\n";
  }

  ASSERT_FLOAT_EQ(0.000059999998, block[0]);
  ASSERT_FLOAT_EQ(0.010982331, block[1]);
  ASSERT_FLOAT_EQ(0.10301149, block[2]);
  ASSERT_FLOAT_EQ(0.38589266, block[3]);
  ASSERT_FLOAT_EQ(0.79383349, block[4]);
  ASSERT_FLOAT_EQ(1, block[5]);
  ASSERT_FLOAT_EQ(0.79383349, block[6]);
  ASSERT_FLOAT_EQ(0.38589266, block[7]);
  ASSERT_FLOAT_EQ(0.10301149, block[8]);
  ASSERT_FLOAT_EQ(0.010982331, block[9]);
  // ASSERT_FLOAT_EQ(0, block[idx++]);
}

using algae::dsp::control::hann;
TEST(Control_Test, CoreWindowTest_hann) {
  size_t N = 10;

  ASSERT_FLOAT_EQ(0, hann<double>(double(0) / double(N)));
  ASSERT_FLOAT_EQ(0.095491506, hann<double>(double(1) / double(N)));
  ASSERT_FLOAT_EQ(0.3454915, hann<double>(double(2) / double(N)));
  ASSERT_FLOAT_EQ(0.65450847, hann<double>(double(3) / double(N)));
  ASSERT_FLOAT_EQ(0.90450847, hann<double>(double(4) / double(N)));
  ASSERT_FLOAT_EQ(1, hann<double>(double(5) / double(N)));
  ASSERT_FLOAT_EQ(0.90450847, hann<double>(double(6) / double(N)));
  ASSERT_FLOAT_EQ(0.65450847, hann<double>(double(7) / double(N)));
  ASSERT_FLOAT_EQ(0.3454915, hann<double>(double(8) / double(N)));
  ASSERT_FLOAT_EQ(0.095491506, hann<double>(double(9) / double(N)));
  // ASSERT_FLOAT_EQ(0, hann<double>(double(idx++) / double(N)));
}

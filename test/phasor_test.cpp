#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::oscillator::computePhaseIncrement;
TEST(Osc_Test, CORE_computePhaseIncrement) {
  float sample_rate = 48000;
  float epsilon = 0.00000001;
  EXPECT_NEAR(0.000020833, computePhaseIncrement<float>(1, sample_rate),
              epsilon);
  EXPECT_NEAR(0.00916667, computePhaseIncrement<float>(440, sample_rate),
              epsilon);
  EXPECT_NEAR(0.052083333, computePhaseIncrement<float>(2500, sample_rate),
              epsilon);
}

using algae::dsp::oscillator::update_phase;
TEST(Osc_Test, CORE_update_phasor) {
  float sample_rate = 48000;
  float period = algae::dsp::math::TwoPi<float>();
  float phi = computePhaseIncrement<float>(4800, sample_rate);
  float phase = 0;
  float epsilon = 0.00001;

  std::array<float, 10> expected_phases = {0.628319, 1.25664, 1.88496,
                                           2.51327,  3.14159, 3.76991,
                                           4.39823,  5.02655, 5.65487};
  for (size_t frame_idx = 0; frame_idx < 10; frame_idx++) {
    phase = update_phase(phase, phi, period);
    EXPECT_NEAR(expected_phases[frame_idx], phase, epsilon);
  }
}

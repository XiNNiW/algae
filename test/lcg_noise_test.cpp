
#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::oscillator::LCGNoise;

TEST(DSP_Test, CoreLCGNoiseTest) {
  const size_t blocksize = 16;

  auto gen = LCGNoise<float>();

  gen.setSeed(9835);

  float signal[blocksize];
  gen.process(blocksize, signal);
  float expected[blocksize] = {-0.997857, -0.498459,  -1.63942,  -0.623714,
                               -0.94913,  -1.09648,   -1.57654,  -1.90976,
                               -1.88481,  -0.0449389, -1.5157,   -1.67362,
                               -0.627096, -1.74047,   -0.269705, -0.110891};

  for (size_t i = 0; i < blocksize; i++) {
    ASSERT_NEAR(expected[i], signal[i], 0.001);
  }
}

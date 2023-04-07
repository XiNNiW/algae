#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::control::ADEnvelope;

TEST(DSP_Test, CoreADTest) {

  ADEnvelope<double> envelope;
  double a = 4;
  double d = 4;
  const double sampleRate = 1000;
  envelope.set(a, d, sampleRate);
  double output[1] = {0};
  auto advance = [&] { envelope.process(1, output); };

  envelope.trigger();
  // attack phase
  advance();
  EXPECT_FLOAT_EQ(0, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.25, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.5, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.75, output[0]);
  advance();
  EXPECT_FLOAT_EQ(1, output[0]);

  // decay phase
  advance();
  EXPECT_FLOAT_EQ(1, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.75, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.5, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.25, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0, output[0]);
}

TEST(DSP_Test, CoreADTest_INIT) {

  ADEnvelope<double> envelope;
  double a = 0;
  double d = 0;
  const double sampleRate = 1000;
  envelope.set(a, d, sampleRate);
  double output[1] = {0};
  auto advance = [&] { envelope.process(1, output); };

  envelope.trigger();
  // attack
  advance();
  EXPECT_FLOAT_EQ(0, output[0]);
  advance();
  EXPECT_FLOAT_EQ(1, output[0]);
  // decay
  advance();
  EXPECT_FLOAT_EQ(1, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0, output[0]);
}

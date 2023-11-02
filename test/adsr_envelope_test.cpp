#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>
using algae::dsp::control::ADSREnvelope;

TEST(DSP_Test, CoreADSRTest) {

  ADSREnvelope<double> envelope;
  double a = 4, d = 4, r = 4;
  double s = 0.5;
  envelope.set(a, d, s, r, 1000);

  EXPECT_EQ(ADSREnvelope<double>::OFF, envelope.stage);

  // ATTACK

  EXPECT_FLOAT_EQ(0, envelope.next(1));
  EXPECT_EQ(ADSREnvelope<double>::ATTACK, envelope.stage);
  EXPECT_FLOAT_EQ(0.25, envelope.next(1));
  EXPECT_FLOAT_EQ(0.5, envelope.next(1));
  EXPECT_FLOAT_EQ(0.75, envelope.next(1));
  EXPECT_EQ(ADSREnvelope<double>::ATTACK, envelope.stage);
  EXPECT_FLOAT_EQ(1, envelope.next(1));

  // DECAY
  EXPECT_EQ(ADSREnvelope<double>::DECAY, envelope.stage);
  EXPECT_FLOAT_EQ(1, envelope.next(1));
  EXPECT_FLOAT_EQ(1 - .125, envelope.next(1));
  EXPECT_FLOAT_EQ(1 - 2 * .125, envelope.next(1));
  EXPECT_FLOAT_EQ(1 - 3 * .125, envelope.next(1));
  EXPECT_EQ(ADSREnvelope<double>::DECAY, envelope.stage);
  EXPECT_FLOAT_EQ(0.5, envelope.next(1));

  // SUSTAIN
  EXPECT_EQ(ADSREnvelope<double>::SUSTAIN, envelope.stage);
  EXPECT_FLOAT_EQ(0.5, envelope.next(1));
  EXPECT_FLOAT_EQ(0.5, envelope.next(1));
  EXPECT_FLOAT_EQ(0.5, envelope.next(1));
  EXPECT_FLOAT_EQ(0.5, envelope.next(1));
  EXPECT_EQ(ADSREnvelope<double>::SUSTAIN, envelope.stage);

  // RELEASE

  EXPECT_FLOAT_EQ(0.5, envelope.next(0));
  EXPECT_EQ(ADSREnvelope<double>::RELEASE, envelope.stage);
  EXPECT_FLOAT_EQ(0.5, envelope.next(0));
  EXPECT_FLOAT_EQ(0.5 - .125, envelope.next(0));
  EXPECT_FLOAT_EQ(0.5 - 2 * .125, envelope.next(0));
  EXPECT_FLOAT_EQ(0.5 - 3 * .125, envelope.next(0));
  EXPECT_FLOAT_EQ(0.5 - 4 * .125, envelope.next(0));
  EXPECT_EQ(ADSREnvelope<double>::OFF, envelope.stage);
  EXPECT_FLOAT_EQ(0, envelope.next(0));
  EXPECT_EQ(ADSREnvelope<double>::OFF, envelope.stage);
}

TEST(DSP_Test, CoreADSRTest_InterruptedDecay) {
  ADSREnvelope<double> envelope;
  double a = 4, d = 4, r = 4;
  double sampleRate = 1000;
  double s = 0.5;
  envelope.set(a, d, s, r, sampleRate);
  double output[1] = {0};

  auto advance = [&]() { envelope.process(1, output); };

  envelope.setGate(1);

  // ATTACK
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

  // DECAY
  advance();
  EXPECT_FLOAT_EQ(1, output[0]);
  advance();
  EXPECT_FLOAT_EQ(1 - .125, output[0]);
  advance();
  EXPECT_FLOAT_EQ(1 - 2 * .125, output[0]);

  envelope.setGate(0);

  advance();
  EXPECT_FLOAT_EQ(0.625, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.5, output[0]);

  advance();
  EXPECT_FLOAT_EQ(0.5, output[0]);

  // RELEASE
  advance();
  EXPECT_FLOAT_EQ(0.5, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.5 - 1 * .125, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.5 - 2 * .125, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.5 - 3 * .125, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0.5 - 4 * .125, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0, output[0]);
}

TEST(DSP_Test, CoreADSRTest_default) {
  ADSREnvelope<double> envelope;

  double output[1] = {0};
  auto advance = [&]() { envelope.process(1, output); };

  envelope.setGate(1);

  // RELEASE
  advance();
  EXPECT_FLOAT_EQ(0, output[0]);
  advance();
  EXPECT_FLOAT_EQ(1, output[0]);
  envelope.setGate(0);
  advance();
  EXPECT_FLOAT_EQ(1, output[0]);
  advance();
  EXPECT_FLOAT_EQ(1, output[0]);
  advance();
  EXPECT_FLOAT_EQ(1, output[0]);
  advance();
  EXPECT_FLOAT_EQ(1, output[0]);
  advance();
  EXPECT_FLOAT_EQ(0, output[0]);
}

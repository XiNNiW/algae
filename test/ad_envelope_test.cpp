#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::control::ADEnvelope;

TEST(DSP_Test, ADEnvelopeTest) {

  ADEnvelope<double> envelope;
  double a = 4;
  double d = 4;
  const double sampleRate = 1000;
  envelope.set(a, d, sampleRate);

  // attack phase
  envelope.trigger();
  EXPECT_FLOAT_EQ(0, envelope.next());
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  EXPECT_FLOAT_EQ(1, envelope.next());

  // decay phase
  EXPECT_FLOAT_EQ(1, envelope.next());
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  EXPECT_FLOAT_EQ(0, envelope.next());
  EXPECT_FLOAT_EQ(0, envelope.next());
}

TEST(DSP_Test, ADEnvelopeTest_setGate) {

  ADEnvelope<double> envelope;
  double a = 4;
  double d = 4;
  const double sampleRate = 1000;
  envelope.set(a, d, sampleRate);

  // attack phase
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(true);

  // decay phase
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0, envelope.next());

  // attack phase
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(true);
  // interrupted decay phase
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(true);
}
TEST(DSP_Test, ADEnvelopeTest_setGate_interruptedAttack) {

  ADEnvelope<double> envelope;
  double a = 4;
  double d = 4;
  const double sampleRate = 1000;
  envelope.set(a, d, sampleRate);

  // interrupted attack phase
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  envelope.setGate(false);

  // decay phase
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.0, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0, envelope.next());
}

TEST(DSP_Test, ADEnvelopeTest_INIT) {

  ADEnvelope<double> envelope;
  double a = 0;
  double d = 0;
  const double sampleRate = 1000;
  envelope.set(a, d, sampleRate);

  envelope.trigger();
  //  attack
  EXPECT_FLOAT_EQ(0, envelope.next());
  EXPECT_FLOAT_EQ(1, envelope.next());
  // decay
  EXPECT_FLOAT_EQ(1, envelope.next());
  EXPECT_FLOAT_EQ(0, envelope.next());
}

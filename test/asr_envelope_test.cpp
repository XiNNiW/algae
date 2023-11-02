#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::control::ASREnvelope;

TEST(DSP_Test, ASREnvelopeTest) {

  ASREnvelope<double> envelope;
  double a = 4;
  double d = 4;
  const double sampleRate = 1000;
  envelope.set(a, d, sampleRate);

  // attack phase
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(0, envelope.next());
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  EXPECT_FLOAT_EQ(1, envelope.next());

  // sustain phase
  EXPECT_FLOAT_EQ(1, envelope.next());
  EXPECT_FLOAT_EQ(1, envelope.next());
  EXPECT_FLOAT_EQ(1, envelope.next());
  EXPECT_FLOAT_EQ(1, envelope.next());
  // decay phase
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(1, envelope.next());
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  EXPECT_FLOAT_EQ(0, envelope.next());
  EXPECT_FLOAT_EQ(0, envelope.next());
}

TEST(DSP_Test, ASREnvelopeTest_setGate) {

  ASREnvelope<double> envelope;
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
  // sustain phase
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  // decay phase
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.5, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0, envelope.next());
  envelope.setGate(false);

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
  // sustain phase
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(true);
  EXPECT_FLOAT_EQ(1, envelope.next());
  // interrupted decay phase
  envelope.setGate(false);
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
TEST(DSP_Test, ASREnvelopeTest_setGate_interruptedAttack) {

  ASREnvelope<double> envelope;
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

  // decay phase
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.75, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.50, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0.25, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0, envelope.next());
  envelope.setGate(false);
  EXPECT_FLOAT_EQ(0, envelope.next());
}

TEST(DSP_Test, ASREnvelopeTest_INIT) {

  ASREnvelope<double> envelope;
  double a = 0;
  double d = 0;
  const double sampleRate = 1000;
  envelope.set(a, d, sampleRate);

  envelope.setGate(true);
  //  attack
  EXPECT_FLOAT_EQ(0, envelope.next());
  EXPECT_FLOAT_EQ(1, envelope.next());
  envelope.setGate(false);
  // decay
  EXPECT_FLOAT_EQ(1, envelope.next());
  EXPECT_FLOAT_EQ(0, envelope.next());
}

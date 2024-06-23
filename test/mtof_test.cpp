#include "algae.h"
#include <gtest/gtest.h>
#include <iostream>

using algae::dsp::math::mtof;

TEST(DSP_Test, CORE_mtof) {
  /*
  72 		C5 	 	523.25
  71 	 	B4 	 	493.88
  70 	 	A#4/Bb4 466.16
  69 	 	A4 	 	440.00
  68 	 	G#4/Ab4 415.30
  67 	 	G4 	 	392.00
  66 	 	F#4/Gb4 369.99
  65 	 	F4 	 	349.23
  64 	 	E4 	 	329.63
  63 	 	D#4/Eb4 311.13
  62 	 	D4 	 	293.66
  61 	 	C#4/Db4 277.18
  60 	 	C4   	261.63
  */

  EXPECT_FLOAT_EQ(261.62558, mtof<double>(60.0));
  EXPECT_FLOAT_EQ(277.18262, mtof<double>(61.0));
  EXPECT_FLOAT_EQ(293.66476, mtof<double>(62.0));
  EXPECT_FLOAT_EQ(311.12698, mtof<double>(63.0));
  EXPECT_FLOAT_EQ(329.62756, mtof<double>(64.0));
  EXPECT_FLOAT_EQ(349.22824, mtof<double>(65.0));
  EXPECT_NEAR(369.984427, mtof<double>(66.0), 0.01);
  EXPECT_FLOAT_EQ(391.99542, mtof<double>(67.0));
  EXPECT_FLOAT_EQ(415.30469, mtof<double>(68.0));
  EXPECT_FLOAT_EQ(440.0, mtof<double>(69.0));
  EXPECT_FLOAT_EQ(466.16376, mtof<double>(70.0));
  EXPECT_FLOAT_EQ(493.8833, mtof<double>(71.0));
  EXPECT_FLOAT_EQ(523.25116, mtof<double>(72.0));

  EXPECT_FLOAT_EQ(8.1757994, mtof<double>(0));
}

using algae::dsp::math::ftom;
TEST(DSP_Test, CORE_ftom) {

  EXPECT_FLOAT_EQ(60.0, ftom<double>(261.62558));
  EXPECT_FLOAT_EQ(61.0, ftom<double>(277.18262));
  EXPECT_FLOAT_EQ(62.0, ftom<double>(293.66476));
  EXPECT_FLOAT_EQ(63.0, ftom<double>(311.12698));
  EXPECT_FLOAT_EQ(64.0, ftom<double>(329.62756));
  EXPECT_FLOAT_EQ(65.0, ftom<double>(349.22824));
  EXPECT_NEAR(66.0, ftom<double>(369.984427), 0.01);
  EXPECT_FLOAT_EQ(67.0, ftom<double>(391.99542));
  EXPECT_FLOAT_EQ(68.0, ftom<double>(415.30469));
  EXPECT_FLOAT_EQ(69.0, ftom<double>(440.0));
  EXPECT_FLOAT_EQ(70.0, ftom<double>(466.16376));
  EXPECT_FLOAT_EQ(71.0, ftom<double>(493.8833));
  EXPECT_FLOAT_EQ(72.0, ftom<double>(523.25116));
}

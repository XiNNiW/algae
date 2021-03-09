#include <gtest/gtest.h>
#include "../src/core/units.h"
#include <iostream>

using algae::dsp::core::units::msToSamples;
using algae::dsp::core::units::mtof;


TEST(DSP_Test, CORE_mstosamps) { 
    
    double ms = 1000.0;
    double sample_rate = 44100;

    ASSERT_FLOAT_EQ(44100.0, msToSamples(ms,sample_rate));

    ms = 500.0;
    sample_rate = 44100;

    ASSERT_FLOAT_EQ(22050.0, msToSamples(ms,sample_rate));

    ms = 1000;
    sample_rate = 8000.0;

    ASSERT_FLOAT_EQ(8000.0, msToSamples(ms,sample_rate));

}

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
    

    EXPECT_FLOAT_EQ(261.63, mtof<double>(60.0));
    EXPECT_FLOAT_EQ(277.18, mtof<double>(61.0));
    EXPECT_FLOAT_EQ(293.66, mtof<double>(62.0));
    EXPECT_FLOAT_EQ(311.13, mtof<double>(63.0));
    EXPECT_FLOAT_EQ(329.63, mtof<double>(64.0));
    EXPECT_FLOAT_EQ(349.23, mtof<double>(65.0));
    EXPECT_FLOAT_EQ(369.99, mtof<double>(66.0));
    EXPECT_FLOAT_EQ(392.0,  mtof<double>(67.0));
    EXPECT_FLOAT_EQ(415.3,  mtof<double>(68.0));
    EXPECT_FLOAT_EQ(440.0,  mtof<double>(69.0));
    EXPECT_FLOAT_EQ(466.16, mtof<double>(70.0));
    EXPECT_FLOAT_EQ(493.88, mtof<double>(71.0));
    EXPECT_FLOAT_EQ(523.25, mtof<double>(72.0));



}



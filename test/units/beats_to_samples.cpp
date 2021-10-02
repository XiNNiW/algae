
#include <gtest/gtest.h>
#include "../../src/core/units.h"
#include <iostream>



using algae::dsp::core::units::beatsToSamples;
TEST(DSP_Test, CORE_beats) { 
    
    double bpm = 120;
    double num_beats = 4;

    ASSERT_FLOAT_EQ(96000, beatsToSamples(num_beats,bpm,48000));

}
#include <gtest/gtest.h>
#include "../../src/core/units.h"
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

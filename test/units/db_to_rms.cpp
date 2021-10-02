#include <gtest/gtest.h>
#include "../../src/core/units.h"
#include <iostream>

using algae::dsp::core::units::dbtorms;
TEST(DSP_Test, CORE_dbtorms) { 
    
    double db = 100;
    double rms = 1;

    ASSERT_FLOAT_EQ(rms, dbtorms(db));

}


#include <gtest/gtest.h>
#include "../../src/core/units.h"
#include <iostream>

using algae::dsp::core::units::dbtopow;
TEST(DSP_Test, CORE_dbtopow) { 
    
    double db = 100;
    double power = 1;

    ASSERT_FLOAT_EQ(power, dbtopow(db));

}


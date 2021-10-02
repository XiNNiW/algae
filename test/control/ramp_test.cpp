#include <gtest/gtest.h>
#include "../../src/core/control.h"
#include <iostream>

using algae::dsp::core::control::ramp_t;
using algae::dsp::core::control::update_ramp;
using algae::dsp::core::control::update_ad;
using algae::dsp::core::control::update_adsr;


TEST(DSP_Test, CoreRampTest) { 
    
    int ramptimeInSamples = 4;
    double initialValue = 0;
    double finalValue = 1;
    ramp_t<double> envState = ramp_t<double>{};

    ASSERT_FLOAT_EQ(0,envState.value);
    ASSERT_EQ(0,envState.index);

    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples);
    EXPECT_FLOAT_EQ(0,envState.value);
    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples);
    EXPECT_FLOAT_EQ(0.25,envState.value);

    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples);
    EXPECT_FLOAT_EQ(0.5,envState.value);

    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples);

    EXPECT_FLOAT_EQ(0.75,envState.value);
    EXPECT_EQ(4,envState.index);

    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples);

    EXPECT_FLOAT_EQ(1,envState.value);
    EXPECT_EQ(5,envState.index);

    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples);

    EXPECT_FLOAT_EQ(1,envState.value);
    EXPECT_EQ(5,envState.index);

}

TEST(DSP_Test, CoreRampTest_Offset) { 
    
    int ramptimeInSamples = 4;
    int offset = 4;
    double initialValue = 0;
    double finalValue = 1;
    ramp_t<double> envState = ramp_t<double>{};

    ASSERT_FLOAT_EQ(0,envState.value);
    ASSERT_EQ(0,envState.index);

    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples, offset);
    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples, offset);
    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples, offset);
    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples, offset);

    ASSERT_FLOAT_EQ(0,envState.value);
    ASSERT_EQ(4,envState.index);

    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples, offset);
    EXPECT_FLOAT_EQ(0,envState.value);
    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples, offset);
    EXPECT_FLOAT_EQ(0.25,envState.value);

    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples, offset);
    EXPECT_FLOAT_EQ(0.5,envState.value);

    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples, offset);

    EXPECT_FLOAT_EQ(0.75,envState.value);
    EXPECT_EQ(8,envState.index);

    envState = update_ramp<double>(envState, initialValue, finalValue, ramptimeInSamples, offset);

    EXPECT_FLOAT_EQ(1,envState.value);
    EXPECT_EQ(9,envState.index);

}

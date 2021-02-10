#include <gtest/gtest.h>
#include "../src/core/control.h"
#include <iostream>

// using algae::dsp::shell::dsp_node;
// using algae::dsp::shell::connect;
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
    EXPECT_EQ(6,envState.index);

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

TEST(DSP_Test, CoreADTest) { 
    
    ramp_t<double> envState = ramp_t<double>{false,0,0};
    int a= 4;
    int d= 4;

    ASSERT_FLOAT_EQ(0,envState.value);
    ASSERT_EQ(0,envState.index);

    //attack phase
    envState = update_ad<double>(envState,a,d); 
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(1,envState.index);
    envState = update_ad<double>(envState,a,d); 
    EXPECT_FLOAT_EQ(0.25,envState.value);
    EXPECT_EQ(2,envState.index);
    envState = update_ad<double>(envState,a,d); 
    EXPECT_FLOAT_EQ(0.5,envState.value);
    EXPECT_EQ(3,envState.index);
    envState = update_ad<double>(envState,a,d); 
    EXPECT_FLOAT_EQ(0.75,envState.value);
    EXPECT_EQ(4,envState.index);
    envState = update_ad<double>(envState,a,d); 
    EXPECT_FLOAT_EQ(1,envState.value);
    EXPECT_EQ(5,envState.index);

    //decay phase

    envState = update_ad<double>(envState, a,d); 
    EXPECT_FLOAT_EQ(0.75,envState.value);
    EXPECT_EQ(6,envState.index);
    envState = update_ad<double>(envState, a,d); 
    EXPECT_FLOAT_EQ(0.5,envState.value);
    EXPECT_EQ(7,envState.index);
    envState = update_ad<double>(envState, a,d); 
    EXPECT_FLOAT_EQ(0.25,envState.value);
    EXPECT_EQ(8,envState.index);
    envState = update_ad<double>(envState, a,d); 
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(9,envState.index);
    envState = update_ad<double>(envState, a,d); 
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(0,envState.index);

}



// TEST(DSP_Test, CoreADSRTest) { 
    
//     ramp_t<double> envState = ramp_t<double>{false,0,0};
//     int a,d,r = 3;
//     double s = 0.5;

//     ASSERT_FLOAT_EQ(0,envState.value);
//     ASSERT_EQ(0,envState.index);

//     envState = update_adsr<double,double>(envState, true, a,d,s,r);
//     // envState = update_adsr<double,double>(envState, true, a,d,s,r);
//     // envState = update_adsr<double,double>(envState, true, a,d,s,r); 
//     EXPECT_FLOAT_EQ(1,envState.value);
//     EXPECT_EQ(3,envState.index);

//     envState = update_adsr<double,double>(envState, true, a,d,s,r);
//     envState = update_adsr<double,double>(envState, true, a,d,s,r);
//     envState = update_adsr<double,double>(envState, true, a,d,s,r); 
//     EXPECT_FLOAT_EQ(0.5,envState.value);
//     EXPECT_EQ(6,envState.index);

//     envState = update_adsr<double,double>(envState, true, a,d,s,r);
//     envState = update_adsr<double,double>(envState, true, a,d,s,r);
//     envState = update_adsr<double,double>(envState, true, a,d,s,r); 
//     EXPECT_FLOAT_EQ(0.5,envState.value);
//     EXPECT_EQ(9,envState.index);

//     envState = update_adsr<double,double>(envState, false, a,d,s,r);
//     envState = update_adsr<double,double>(envState, false, a,d,s,r);
//     envState = update_adsr<double,double>(envState, false, a,d,s,r); 
//     EXPECT_FLOAT_EQ(0,envState.value);
//     EXPECT_EQ(12,envState.index);



// }





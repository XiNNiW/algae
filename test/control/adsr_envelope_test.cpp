#include <gtest/gtest.h>
#include "../../src/core/control.h"
#include <iostream>

using algae::dsp::core::control::ramp_t;
using algae::dsp::core::control::update_ramp;
using algae::dsp::core::control::update_ad;
using algae::dsp::core::control::update_adsr;


using algae::dsp::core::control::adsr_t;

TEST(DSP_Test, CoreADSRTest) { 
    
    adsr_t<double> envState = adsr_t<double>{ramp_t<double>{0,0},0};
    int a=4,d=4,r = 4;
    double s = 0.5;
    int call_index=0;

    ASSERT_FLOAT_EQ(0,envState.env.value);
    ASSERT_EQ(call_index,envState.env.index);

    //ATTACK
    envState = update_adsr<double>(envState, true, a,d,s,r);
    call_index++;
    EXPECT_FLOAT_EQ(0,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r);
    call_index++;
    EXPECT_FLOAT_EQ(0.25,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.5,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.75,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(1,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);

    //DECAY
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(1-.125,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(1-2*.125,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(1-3*.125,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.5,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);

    //SUSTAIN
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    EXPECT_FLOAT_EQ(0.5,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    EXPECT_FLOAT_EQ(0.5,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    EXPECT_FLOAT_EQ(0.5,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    EXPECT_FLOAT_EQ(0.5,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);

    //RELEASE
    envState = update_adsr<double>(envState, false, a,d,s,r);
    call_index++;
    EXPECT_FLOAT_EQ(0.5-.125,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, false, a,d,s,r);
    call_index++;
    EXPECT_FLOAT_EQ(0.5-2*.125,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, false, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.5-3*.125,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, false, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.5-4*.125,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);

}

TEST(DSP_Test, CoreADSRTest_InterruptedDecay) { 
    
    adsr_t<double> envState = adsr_t<double>{ramp_t<double>{0,0},0};
    int a=4,d=4,r = 4;
    double s = 0.5;
    int call_index=0;

    ASSERT_FLOAT_EQ(0,envState.env.value);
    ASSERT_EQ(call_index,envState.env.index);

    //ATTACK
    envState = update_adsr<double>(envState, true, a,d,s,r);
    call_index++;
    EXPECT_FLOAT_EQ(0,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r);
    call_index++;
    EXPECT_FLOAT_EQ(0.25,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.5,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.75,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(1,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);

    //DECAY
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(1-.125,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, true, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(1-2*.125,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);

    //RELEASE
    envState = update_adsr<double>(envState, false, a,d,s,r);
    call_index++;
    EXPECT_FLOAT_EQ(0.75-0*.1875,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, false, a,d,s,r);
    call_index++;
    EXPECT_FLOAT_EQ(0.75-0*.1875,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, false, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.75-1*.1875,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, false, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.75-2*.1875,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, false, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.75-3*.1875,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, false, a,d,s,r); 
    call_index++;
    EXPECT_FLOAT_EQ(0.75-4*.1875,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);

}

TEST(DSP_Test, CoreADSRTest_default) { 
    
    adsr_t<double> envState = adsr_t<double>{ramp_t<double>{0,0},0};
    int a=0,d=0,r = 0;
    double s = 0.5;
    int call_index=0;

    ASSERT_FLOAT_EQ(0,envState.env.value);
    ASSERT_EQ(call_index,envState.env.index);

    //RELEASE
    envState = update_adsr<double>(envState, false, a,d,s,r);
    EXPECT_FLOAT_EQ(0,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);
    envState = update_adsr<double>(envState, false, a,d,s,r);
    EXPECT_FLOAT_EQ(0,envState.env.value);
    EXPECT_EQ(call_index,envState.env.index);

}





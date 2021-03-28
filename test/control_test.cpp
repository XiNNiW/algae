#include <gtest/gtest.h>
#include "../src/core/control.h"
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

TEST(DSP_Test, CoreADTest) { 
    
    ramp_t<double> envState = ramp_t<double>{0,0};
    int a= 4;
    int d= 4;
    int index = 0;
    auto advance = [&]{envState = update_ad<double>(envState,a,d); };

    ASSERT_FLOAT_EQ(0,envState.value);
    ASSERT_EQ(index,envState.index);

    //attack phase
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.25,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.5,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.75,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(1,envState.value);
    EXPECT_EQ(index,envState.index);

    //decay phase

    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.75,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.5,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.25,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(index,envState.index);

}

TEST(DSP_Test, CoreADTest2) { 
    
    ramp_t<double> envState = ramp_t<double>{0,0};
    int a= 4;
    int d= 4;
    int index = 0;

    ASSERT_FLOAT_EQ(0,envState.value);
    ASSERT_EQ(index,envState.index);

    
    auto advance = [&](){envState = update_ad<double>(envState,0,a,1,d,0); };

    //attack phase
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.25,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.5,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.75,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(1,envState.value);
    EXPECT_EQ(index,envState.index);

    //decay phase

    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.75,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.5,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0.25,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 

    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(index,envState.index);

}

TEST(DSP_Test, CoreADTest_INIT) { 
    
    ramp_t<double> envState = ramp_t<double>{0,0};
    int a= 0;
    int d= 0;
    int index = 0;

    ASSERT_FLOAT_EQ(0,envState.value);
    ASSERT_EQ(index,envState.index);

    
    auto advance = [&](){envState = update_ad<double>(envState,0,a,1,d,0); };
    advance(); 
    index++;
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(index,envState.index);
    advance(); 
    EXPECT_FLOAT_EQ(0,envState.value);
    EXPECT_EQ(index,envState.index);
   
}
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





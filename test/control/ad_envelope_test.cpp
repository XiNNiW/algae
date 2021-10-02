#include <gtest/gtest.h>
#include "../../src/core/control.h"
#include <iostream>

using algae::dsp::core::control::ramp_t;
using algae::dsp::core::control::update_ad;
using algae::dsp::core::control::update_adsr;



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
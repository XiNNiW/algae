#include <gtest/gtest.h>
#include <iostream>

#include "../../src/core/audio_block.h"
#include "../../src/core/oscillator.h"

using algae::dsp::core::AudioBlock;

using algae::dsp::core::oscillator::stk_blit_square_t;
using algae::dsp::core::oscillator::stk_blit_square;
using algae::dsp::core::oscillator::process;
TEST(DSP_Test, CORE_stk_blit_square_process) {
 // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    constexpr float SR = 48000;


    stk_blit_square_t<float> osc = stk_blit_square<float,float>(440,SR);
    AudioBlock<float,BLOCKSIZE> output=AudioBlock<float,BLOCKSIZE>::empty();


    std::tie(osc, output) = process<float, BLOCKSIZE>(osc);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-10.1);
        EXPECT_LT(output[i],10.1);
    }
}
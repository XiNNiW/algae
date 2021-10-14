#include <gtest/gtest.h>
#include "../src/shell/dsp_graph.h"
#include "../src/shell/math_functions.h"
#include "../src/core/math_functions.h"
#include "../src/core/audio_block.h"
#include "../src/core/oscillator.h"
#include <iostream>

using algae::dsp::shell::dsp_node;
using algae::dsp::shell::connect;
using algae::dsp::shell::math::constant_node;

TEST(DSP_Test, ConstantNode) { 
    auto number = constant_node<double,double>(5);

    ASSERT_EQ(5,number.getOutputValue(0));

    number.update();

    ASSERT_EQ(5,number.getOutputValue(0));

}

using algae::dsp::core::AudioBlock;
using algae::dsp::core::math::tanh_approx_pade;
using algae::dsp::core::oscillator::noise;
TEST(DSP_Test, block_tanh_approximation_clamps_signal) { 
    const auto BLOCKSIZE = 64;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = noise<float, BLOCKSIZE>();
    input *= 4;
    AudioBlock<float,BLOCKSIZE> output = tanh_approx_pade<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_GT( output[idx],-1.01);
        EXPECT_LT( output[idx],1.01);
    }

}

TEST(DSP_Test, block_tanh_approximation) { 
    const auto BLOCKSIZE = 64;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = noise<float, BLOCKSIZE>();
    AudioBlock<float,BLOCKSIZE> expected;
    for(size_t idx=0; idx<BLOCKSIZE; idx++){
        expected[idx] = tanh(input[idx]);
    }
    AudioBlock<float,BLOCKSIZE> actual = tanh_approx_pade<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.1);
    }

}

TEST(DSP_Test, block_sin_op) { 
    const auto BLOCKSIZE = 64;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = noise<float, BLOCKSIZE>();
    AudioBlock<float,BLOCKSIZE> expected;
    for(size_t idx=0; idx<BLOCKSIZE; idx++){
        expected[idx] = sin(input[idx]);
    }
    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::sin<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}

TEST(DSP_Test, block_cos_op) { 
    const auto BLOCKSIZE = 64;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = noise<float, BLOCKSIZE>();
    AudioBlock<float,BLOCKSIZE> expected;
    for(size_t idx=0; idx<BLOCKSIZE; idx++){
        expected[idx] = cos(input[idx]);
    }
    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::cos<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}

TEST(DSP_Test, block_atan_op) { 
    const auto BLOCKSIZE = 64;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = noise<float, BLOCKSIZE>();
    AudioBlock<float,BLOCKSIZE> expected;
    for(size_t idx=0; idx<BLOCKSIZE; idx++){
        expected[idx] = atan(input[idx]);
    }
    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::atan<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}

TEST(DSP_Test, block_tan_op) { 
    const auto BLOCKSIZE = 64;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = noise<float, BLOCKSIZE>();
    AudioBlock<float,BLOCKSIZE> expected;
    for(size_t idx=0; idx<BLOCKSIZE; idx++){
        expected[idx] = tan(input[idx]);
    }
    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::tan<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}


TEST(DSP_Test, block_sqrt_op) { 
    const auto BLOCKSIZE = 64;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = noise<float, BLOCKSIZE>();
    for(size_t idx=0; idx<BLOCKSIZE; idx++){
        input[idx] = 100*abs(input[idx]);
    }
    AudioBlock<float,BLOCKSIZE> expected;
    for(size_t idx=0; idx<BLOCKSIZE; idx++){
        expected[idx] = sqrt(input[idx]);
    }
    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::sqrt<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}

TEST(DSP_Test, block_abs_op) { 
    const auto BLOCKSIZE = 64;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = noise<float, BLOCKSIZE>();
    AudioBlock<float,BLOCKSIZE> expected;
    for(size_t idx=0; idx<BLOCKSIZE; idx++){
        expected[idx] = abs(input[idx]);
    }
    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::abs<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}

using algae::dsp::core::makeBlock;
TEST(DSP_Test, block_pow_op) { 
    const auto BLOCKSIZE = 4;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> x = makeBlock<float, BLOCKSIZE>({3,0.4,0.88,0.1});
    AudioBlock<float,BLOCKSIZE> y = makeBlock<float, BLOCKSIZE>({0,0.5,-1,4});
    AudioBlock<float,BLOCKSIZE> expected;
    for(size_t idx=0; idx<BLOCKSIZE; idx++){
        expected[idx] = pow(x[idx],y[idx]);
    }
    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::pow<float, BLOCKSIZE>(x,y);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}

TEST(DSP_Test, block_clip_op) { 
    const auto BLOCKSIZE = 4;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = makeBlock<float, BLOCKSIZE>({0.1, -0.4, 1.1, -45});
    AudioBlock<float,BLOCKSIZE> expected = makeBlock<float, BLOCKSIZE>({0.1, -0.4, 1.0, -1.0});

    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::clip<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}

using algae::dsp::core::makeBlock;
TEST(DSP_Test, block_wrap_op) { 
    const auto BLOCKSIZE = 4;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = makeBlock<float, BLOCKSIZE>({0.1, -0.4, 1.1, -1.1});
    AudioBlock<float,BLOCKSIZE> expected = makeBlock<float, BLOCKSIZE>({0.1, 0.6, 0.1, 0.9});

    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::wrap<float, BLOCKSIZE>(input);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}


using algae::dsp::core::makeBlock;
TEST(DSP_Test, block_clamp_op) { 
    const auto BLOCKSIZE = 4;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = makeBlock<float, BLOCKSIZE>({0.1, -0.4, 1.1, -1.1});
    AudioBlock<float,BLOCKSIZE> low = makeBlock<float, BLOCKSIZE>({-0.5,-0.5,-0.5,-0.5});
    AudioBlock<float,BLOCKSIZE> high = makeBlock<float, BLOCKSIZE>({0.5,0.5,0.5,0.5});
    AudioBlock<float,BLOCKSIZE> expected = makeBlock<float, BLOCKSIZE>({0.1, -0.4, 0.5, -0.5});

    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::clamp<float, BLOCKSIZE>(input, low, high);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}

using algae::dsp::core::makeBlock;
TEST(DSP_Test, block_wrapto_op) { 
    const auto BLOCKSIZE = 4;
    const auto SR = 48000.0;

    AudioBlock<float,BLOCKSIZE> input = makeBlock<float, BLOCKSIZE>({0.1, -0.4, 1.1, -1.1});
    AudioBlock<float,BLOCKSIZE> wrapPoint = makeBlock<float, BLOCKSIZE>({-0.5,-0.5,-0.5,-0.5});
    AudioBlock<float,BLOCKSIZE> expected = makeBlock<float, BLOCKSIZE>({-0.05, -0.3, -0.05, -0.449});

    AudioBlock<float,BLOCKSIZE> actual = algae::dsp::core::math::wrapto<float, BLOCKSIZE>(input, wrapPoint);
    
    for(size_t idx; idx<BLOCKSIZE; idx++){
        EXPECT_NEAR(expected[idx],actual[idx],0.001);
    }

}
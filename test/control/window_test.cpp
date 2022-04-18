#include <gtest/gtest.h>
#include "algae.h"
#include <iostream>

using algae::dsp::core::control::blackman_harris;
TEST(Control_Test, CoreWindowTest_BlackmanHarris) { 
    size_t N = 10;
    size_t idx = 0;

    ASSERT_FLOAT_EQ(0.000059999998, blackman_harris<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.010982331, blackman_harris<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.10301149, blackman_harris<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.38589266, blackman_harris<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.79383349, blackman_harris<double>(idx++,N));
    ASSERT_FLOAT_EQ(1, blackman_harris<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.79383349, blackman_harris<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.38589266, blackman_harris<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.10301149, blackman_harris<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.010982331, blackman_harris<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.000059999998, blackman_harris<double>(idx++,N));
}


using algae::dsp::core::AudioBlock;
using algae::dsp::core::control::apply_window;
TEST(Control_Test, CoreWindowTest_window_block) { 
    constexpr size_t N = 10;
    AudioBlock<double,N> block = AudioBlock<double,N>::empty();
    block += 1;

    block = apply_window<double, N, blackman_harris>(block);
    for(size_t i=0; i<N; i++){
        std::cout<< block[i]<<"\n";
    }
    
    size_t idx = 0;
    ASSERT_FLOAT_EQ(0.000059999998,     block[idx++]);
    ASSERT_FLOAT_EQ(0.010982331,        block[idx++]);
    ASSERT_FLOAT_EQ(0.10301149,         block[idx++]);
    ASSERT_FLOAT_EQ(0.38589266,         block[idx++]);
    ASSERT_FLOAT_EQ(0.79383349,         block[idx++]);
    ASSERT_FLOAT_EQ(1,                  block[idx++]);
    ASSERT_FLOAT_EQ(0.79383349,         block[idx++]);
    ASSERT_FLOAT_EQ(0.38589266,         block[idx++]);
    ASSERT_FLOAT_EQ(0.10301149,         block[idx++]);
    ASSERT_FLOAT_EQ(0.010982331,        block[idx++]);
    ASSERT_FLOAT_EQ(0,                  block[idx++]);
    
}

using algae::dsp::core::control::hann;
TEST(Control_Test, CoreWindowTest_hann) { 
    size_t N = 10;
    size_t idx = 0;

    ASSERT_FLOAT_EQ(0, hann<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.095491506,    hann<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.3454915,     hann<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.65450847,     hann<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.90450847,     hann<double>(idx++,N));
    ASSERT_FLOAT_EQ(1,              hann<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.90450847,     hann<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.65450847,     hann<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.3454915,     hann<double>(idx++,N));
    ASSERT_FLOAT_EQ(0.095491506,    hann<double>(idx++,N));
    ASSERT_FLOAT_EQ(0,    hann<double>(idx++,N));
}
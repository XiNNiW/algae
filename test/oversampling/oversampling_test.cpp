// #include <gtest/gtest.h>
// #include <iostream>

// #include "../../src/core/oversampling.h"
// #include "oversampling_test.h"

// using algae::dsp::core::oversampling::upsample_zero_pad;

// TEST(Oversampling_Test, CORE_upsample_zero_pad) {
//     const size_t BLOCKSIZE = 10;
//     float in[BLOCKSIZE] = {1,2,3,4,5,6,7,8,9,10};
//     const size_t oversampling_factor = 4;
//     float expected[BLOCKSIZE*oversampling_factor] = {1,0,0,0,2,0,0,0,3,0,0,0,4,0,0,0,5,0,0,0,6,0,0,0,7,0,0,0,8,0,0,0,9,0,0,0,10,0,0,0};
//     float actual[BLOCKSIZE*oversampling_factor];
    
//     upsample_zero_pad<float>(in, BLOCKSIZE, oversampling_factor, actual);

//     for(size_t idx=0; idx<BLOCKSIZE*oversampling_factor; idx++)
//         EXPECT_EQ(expected[idx], actual[idx]);


// }

// TEST(Oversampling_Test, CORE_upsample_interpolate) {
//     constexpr BLOCKSIZE = 100;
//     constexpr oversampling_factor = 4;
//     double actual[BLOCKSIZE]
//     const double SR = 48000;

//     interpolator_t<double, oversampling_factor> filter = interpolator(SR); 

//     filter = upsample_interpolate<double, oversampling_factor>(filter, signal.data(), BLOCKSIZE, oversampling_factor, actual);

//     for(size_t idx=0; idx<BLOCKSIZE*oversampling_factor; idx++)
//         EXPECT_EQ(signal_over4x[idx], actual[idx]);


// }
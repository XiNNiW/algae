#include <gtest/gtest.h>
#include <iostream>

#include "../../src/core/audio_block.h"
#include "../../src/core/oscillator/poly_blep.h"
#include "../frequency_test_utilities.h"

using algae::dsp::core::AudioBlock;
using algae::dsp::core::oscillator::blep;
TEST(Osc_Test, CORE_blep) {
    auto epsilon = 0.001;
    EXPECT_NEAR(-0.25,blep(1.0,2.0),epsilon);
    EXPECT_NEAR(-0.5708,blep(1.1,4.5),epsilon);
    EXPECT_NEAR(4,blep(2.0,1.0),epsilon);
    EXPECT_NEAR(9.5537,blep(3.3,1.1),epsilon);
    EXPECT_NEAR(1,blep(1.0,1.0),epsilon);
    EXPECT_NEAR(0,blep(0.0,0.0),epsilon);
}

using algae::dsp::core::AudioBlock;
using algae::dsp::core::oscillator::blamp;
TEST(Osc_Test, CORE_blamp) {
    auto epsilon = 0.001;
    EXPECT_NEAR(0.041666,blamp(1.0,2.0),epsilon);
    EXPECT_NEAR(0.143773,blamp(1.1,4.5),epsilon);
    EXPECT_NEAR(2.666666,blamp(2.0,1.0),epsilon);
    EXPECT_NEAR(9.843225,blamp(3.3,1.1),epsilon);
    EXPECT_NEAR(0.333333,blamp(1.0,1.0),epsilon);
    EXPECT_NEAR(0,blamp(0.0,0.0),epsilon);
}

using algae::dsp::core::simd::simd_traits;
using algae::dsp::core::simd::load_a;
using algae::dsp::core::simd::store_a;
TEST(Osc_Test, CORE_blep__simd_float) {
    constexpr size_t num_cases = 16;
    const auto epsilon = 0.001;
    typedef typename simd_traits<float>::type sample_vec;
    constexpr size_t vec_size = simd_traits<float>::size;

    float t[num_cases]         = {1.0,1.1,2.0,3.3,1.0,0.0};
    float dt[num_cases]        = {2.0,4.5,1.0,1.1,1.0,0.0};
    float expected[num_cases]  = {-0.25,-0.5708,4,9.5537,1,0};
    float actual[num_cases];

    for(size_t idx=0; idx<num_cases/vec_size; idx++){
        sample_vec tv  = load_a(&t[idx*vec_size] );
        sample_vec dtv = load_a(&dt[idx*vec_size]);
        auto next = blep<sample_vec>(tv,dtv);
        store_a(&actual[idx*vec_size], next);
    }

    for(size_t idx=0; idx<num_cases; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],epsilon);
    
}

// TEST(Osc_Test, CORE_blep__simd_double) {
//     constexpr size_t num_cases = 16;
//     const auto epsilon = 0.001;
//     typedef typename simd_traits<double>::type sample_vec;
//     constexpr size_t vec_size = simd_traits<double>::size;

//     double t[num_cases]         = {1.0,1.1,2.0,3.3,1.0,0.0};
//     double dt[num_cases]        = {2.0,4.5,1.0,1.1,1.0,0.0};
//     double expected[num_cases]  = {-0.25,-0.5708,4,9.5537,1,0};
//     double actual[num_cases];

//     for(size_t idx=0; idx<num_cases/vec_size; idx++){
//         sample_vec tv  = load_a(&t[idx*vec_size] );
//         sample_vec dtv = load_a(&dt[idx*vec_size]);
//         auto next = blep<sample_vec>(tv,dtv);
//         store_a(&actual[idx*vec_size], next);
//     }

//     for(size_t idx=0; idx<num_cases; idx++)
//         EXPECT_NEAR(expected[idx],actual[idx],epsilon);
    
// }
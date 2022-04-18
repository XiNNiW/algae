#include <gtest/gtest.h>
#include "algae.h"

using algae::dsp::core::analysis::lpc_matrix_create;
using algae::dsp::core::analysis::lpc_predict;

TEST(Analysis_Test, lpc_matrix_create) {

    std::array<double, 15> signal = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    std::array<double, 6*10> expected = {
        1,2,3,4,5,6,
        2,3,4,5,6,7,
        3,4,5,6,7,8,
        4,5,6,7,8,9,
        5,6,7,8,9,10,
        6,7,8,9,10,11,
        7,8,9,10,11,12,
        8,9,10,11,12,13,
        9,10,11,12,13,14,
        10,11,12,13,14,15
    };
    std::array<double, 6*10> actual = lpc_matrix_create<double, 6, 10>(signal);

    for(size_t idx=0; idx<6*10; idx++)
        EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
} 

// TEST(Analysis_Test, lpc_predict) {

//     std::array<double, 23> signal = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.4,0,-0.4,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0};

//     std::array<double, 6*16> signal_matrix = lpc_matrix_create<double, 6, 16>(signal);
//     std::array<double, 6> expected = {0,0,0,0,0,0};
//     std::array<double, 6> actual = lpc_predict<double, 6, 10>(signal_matrix);

//     for(size_t idx=0; idx<6; idx++)
//         EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
// } 
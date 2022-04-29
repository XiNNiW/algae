#include <gtest/gtest.h>
#include "algae.h"

using algae::dsp::core::analysis::lpc_matrix_create;
using algae::dsp::core::analysis::lpc_predict;

TEST(Analysis_Test, lpc_matrix_create) {

    std::array<double, 10> signal = {1,2,3,4,5,6,7,8,9,10};
    std::array<double, 6*10> expected = {
        1,2,3,4,5,6,
        2,3,4,5,6,7,
        3,4,5,6,7,8,
        4,5,6,7,8,9,
        5,6,7,8,9,10,
        6,7,8,9,10,0,
        7,8,9,10,0,0,
        8,9,10,0,0,0,
        9,10,0,0,0,0,
        10,0,0,0,0,0
    };
    std::array<double, 6*10> actual = lpc_matrix_create<double, 6, 10>(signal);

    for(size_t idx=0; idx<6*10; idx++)
        EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
} 

// using algae::dsp::core::analysis::lpc_analyze;
// TEST(Analysis_Test, lpc_predict) {
// //      acdata = acorr(blk, order)
// //   coeffs = pinv(toeplitz(acdata[:-1])) * -matrix(acdata[1:]).T
// //   coeffs = coeffs.T.tolist()[0]
//     constexpr size_t POLES = 7;
//     constexpr size_t BUFFERSIZE = 23;

//     std::array<double, BUFFERSIZE> signal = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.4,0,-0.4,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0};
//     std::array<double, POLES> expected = {1.0, -1.52256187, 0.57588716, -0.06477903,  0.61648217, -0.77942572, 0.35634188};

//     std::array<double, POLES> actual = lpc_analyze<double, POLES, POLES>(signal);

//     for(size_t idx=0; idx<POLES; idx++)
//         EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
// } 

using algae::dsp::core::analysis::lpc_analyze;
using algae::dsp::core::analysis::lpc_levinson;
TEST(Analysis_Test, lpc_predict) {

    constexpr size_t ORDER = 7;
    constexpr size_t BUFFERSIZE = 23;

    std::array<double, BUFFERSIZE> signal = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.4,0,-0.4,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0};
    std::array<double, ORDER> expected = {1 , -1.52387, 0.563304, -0.0530413, 0.601311, -0.735489, 0.312263};

    double actual[ORDER];
    double k[ORDER];
    double e;
    lpc_levinson<double>(signal.data(), signal.size(), ORDER, actual, k, &e);


    for(size_t idx=0; idx<ORDER; idx++)
        EXPECT_NEAR(expected[idx], actual[idx],0.0001);
} 
#include <gtest/gtest.h>
#include "algae.h"

using algae::dsp::core::analysis::lpc_levinson;
TEST(Analysis_Test, lpc_analyze) {

    constexpr size_t ORDER = 7;
    constexpr size_t BUFFERSIZE = 23;

    std::array<double, BUFFERSIZE> signal = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.4,0,-0.4,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0};
    std::array<double, ORDER> expected = {1 , -1.52387, 0.563304, -0.0530413, 0.601311, -0.735489, 0.312263};
    std::array<double, ORDER-1> expected_reflection_coeffs = {-0.9169435215946841, 0.6901906412478248, 0.177944633676733, 0.06411814171771187, -0.28769498558073436, 0.3122628390363972};
    double expected_error = 0.4006532762441092;

    double actual[ORDER];
    double k[ORDER];
    double actual_err = lpc_levinson<double>(signal.data(), signal.size(), ORDER, actual, k);

    for(size_t idx=0; idx<ORDER; idx++)
        EXPECT_NEAR(expected[idx], actual[idx],0.0001);
    

    for(size_t idx=0; idx<ORDER-1; idx++)
        EXPECT_NEAR(expected_reflection_coeffs[idx], k[idx+1], 0.0001);

    EXPECT_NEAR(expected_error, actual_err, 0.0001);
} 


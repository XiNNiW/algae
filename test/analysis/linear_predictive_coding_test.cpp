#include <gtest/gtest.h>
#include "algae.h"

using algae::dsp::core::analysis::lpc_analyze;
using algae::dsp::core::analysis::lpc_t;
using algae::dsp::core::AudioBlock;
using algae::dsp::core::makeBlock;

TEST(Analysis_Test, lpc_analyze_test) {

    constexpr size_t ORDER = 7;
    constexpr size_t BUFFERSIZE = 23;

    AudioBlock<double, BUFFERSIZE> signal = makeBlock<double, BUFFERSIZE>({0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.4,0,-0.4,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0});
    std::array<double, ORDER> expected_filter_coeffs= {1 , -1.52387, 0.563304, -0.0530413, 0.601311, -0.735489, 0.312263};
    std::array<double, ORDER-1> expected_reflection_coeffs = {-0.9169435215946841, 0.6901906412478248, 0.177944633676733, 0.06411814171771187, -0.28769498558073436, 0.3122628390363972};
    double expected_error = 0.4006532762441092;

    lpc_t<double, ORDER> actual = lpc_analyze<double,ORDER,BUFFERSIZE>(signal);

    for(size_t idx=0; idx<ORDER; idx++)
        EXPECT_NEAR(expected_filter_coeffs[idx], actual.filter_coefficients[idx],0.0001);

    for(size_t idx=0; idx<ORDER-1; idx++)
        EXPECT_NEAR(expected_reflection_coeffs[idx], actual.reflection_coefficients[idx+1], 0.0001);

    EXPECT_NEAR(expected_error, actual.error, 0.0001);
} 

using algae::dsp::core::analysis::lpc;
using algae::dsp::core::oscillator::noise;

TEST(Analysis_Test, lpc_process) {

    constexpr size_t ORDER = 7;
    constexpr size_t BUFFERSIZE = 16;

    AudioBlock<double, BUFFERSIZE> signal = noise<double, BUFFERSIZE>();
    AudioBlock<double, BUFFERSIZE> carrier = noise<double, BUFFERSIZE>();

    AudioBlock<double, BUFFERSIZE> actual = lpc<double,ORDER,BUFFERSIZE>(signal, carrier);

    for(size_t idx=0; idx<BUFFERSIZE; idx++){
        EXPECT_LT(actual[idx],2);
        EXPECT_GT(actual[idx],-2);
    }
    
    
} 


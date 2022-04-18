#include <gtest/gtest.h>
#include "../../src/shell/oscillator.h"
#include "../../src/shell/filter.h"
#include <iostream>
#include "../frequency_test_utilities.h"

using algae::dsp::core::AudioBlock;
using algae::dsp::core::filter::process;
using algae::dsp::core::oscillator::noise;


using algae::dsp::core::filter::biquad_t;
using algae::dsp::core::filter::lowpass;

using algae::dsp::core::filter::chaotic_resonator_t;
using algae::dsp::core::filter::update_coefficients;
// using algae::dsp::core::filter::process_chaos;
using algae::dsp::core::oscillator::cos_t;

TEST(Filter_Test, CORE_chaotic_cos_bandpass) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    chaotic_resonator_t<double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    //NOTE: for fairly normal values of gain parameters and normal signals, the reson algorithm will clip
    //is there a better way to structure the parameters to prevent this?
    //some filter designs divide out the gain introduced by the peak... however... sometimes you want a boost
    filter.resonator = update_coefficients<double,double>(filter.resonator, 220.0, 0.5, 0.25, 48000.0);
    filter.chaos_gain = 0.5;
    filter.feedback_amt = 1.0;

    for(size_t i = 0; i<BLOCKSIZE; i++){
        input[i] = 0.125*noise<double>();
    }

    std::tie(filter,output) = process<double,cos_t<double,0>::lookup,BLOCKSIZE>(filter, input);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
    
}

using algae::dsp::core::filter::reson_bp;
TEST(Filter_Test, CORE_chaotic_bandpass_amplitude_response_low_feedback) { 
    constexpr size_t NUM_BINS = 32;
    chaotic_resonator_t<double> filter;

    filter.feedback_amt = 0;
    filter.chaos_gain = 0;
    filter.resonator = reson_bp<double,double>( 3420.0, 0.5, 1, 48000.0);
    
    std::array<double, NUM_BINS> amplitude_response={};
    
    enum complex {RE, IM};
    const size_t N = 2*NUM_BINS;
    fftw_complex * in;
    fftw_complex *out;
    fftw_plan p;
   
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    
    AudioBlock<double, N> impulse = AudioBlock<double, N>();
    impulse[0] = 1;
    std::tie(filter,impulse) = process<double,cos_t<double,0>::lookup,N>(filter, impulse);
    for(size_t idx=0; idx<N; idx++){
        in[idx][RE]= impulse[idx];
        in[idx][IM]= 0;

    }

    fftw_execute(p);

    for(size_t idx=0; idx<NUM_BINS; idx++){
        amplitude_response[idx] = (sqrt(out[idx][RE]*out[idx][RE] + out[idx][IM]*out[idx][IM]));    

    }

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
    std::array<double, NUM_BINS> expected_response = {
        1.09816,
        1.05687,
        0.951601,
        0.820358,
        0.693161,
        0.583642,
        0.494267,
        0.422731,
        0.365551,
        0.319491,
        0.281942,
        0.250916,
        0.224928,
        0.202872,
        0.183927,
        0.167473,
        0.153044,
        0.140281,
        0.12891,
        0.118719,
        0.109544,
        0.101261,
        0.0937749,
        0.0870178,
        0.0809439,
        0.0755275,
        0.0707605,
        0.066651,
        0.0632198,
        0.0604977,
        0.0585194,
        0.0573169
    };
    for(size_t bin_idx=0; bin_idx<NUM_BINS; bin_idx++){
        // std::cout << amplitude_response[bin_idx] << std::endl;
        EXPECT_NEAR(expected_response[bin_idx], amplitude_response[bin_idx],0.01);
    }
    // EXPECT_FALSE(true);
}


using algae::dsp::core::filter::reson_bp;
TEST(Filter_Test, CORE_chaotic_bandpass_is_noisy_high_feedback) { 
    constexpr size_t NUM_BINS = 32;
    chaotic_resonator_t<double> filter;

    filter.feedback_amt = 10000;
    filter.chaos_gain = 1;
    filter.resonator = reson_bp<double,double>( 3420.0, 0.5, 1, 48000.0);
    
    std::array<double, NUM_BINS> amplitude_response={};
    
    enum complex {RE, IM};
    const size_t N = 2*NUM_BINS;
    fftw_complex * in;
    fftw_complex *out;
    fftw_plan p;
   
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    
    AudioBlock<double, N> impulse = AudioBlock<double, N>();
    impulse[0] = 1;
    std::tie(filter,impulse) = process<double,cos_t<double,0>::lookup,N>(filter, impulse);
    for(size_t idx=0; idx<N; idx++){
        in[idx][RE]= impulse[idx];
        in[idx][IM]= 0;

    }

    fftw_execute(p);

    double max = 0;
    for(size_t idx=0; idx<NUM_BINS; idx++){
        if(amplitude_response[idx] > max)
            max = amplitude_response[idx];

    }
    if(max <= 0) max = 1;
    for(size_t idx=0; idx<NUM_BINS; idx++){
        amplitude_response[idx] = 10*log10(sqrt(out[idx][RE]*out[idx][RE] + out[idx][IM]*out[idx][IM])/max);    
        
    }

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
    
    for(size_t bin_idx=0; bin_idx<NUM_BINS; bin_idx++){
        // std::cout << amplitude_response[bin_idx] << std::endl;
        EXPECT_GT( amplitude_response[bin_idx],-24);
    }
    // EXPECT_FALSE(true);
}
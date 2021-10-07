#include <gtest/gtest.h>
#include "../../src/core/audio_block.h"
#include "../../src/core/oscillator.h"
#include "../../src/core/filter.h"
#include "../frequency_test_utilities.h"

#include <iostream>

using algae::dsp::core::filter::reson_bp_t;
using algae::dsp::core::filter::update_coefficients;
using algae::dsp::core::filter::process;
using algae::dsp::core::oscillator::noise;
using algae::dsp::core::AudioBlock;

TEST(DSP_Test, CORE_reson_bandpass) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    reson_bp_t<double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    //NOTE: for fairly normal values of gain parameters and normal signals, the reson algorithm will clip
    //is there a better way to structure the parameters to prevent this?
    //some filter designs divide out the gain introduced by the peak... however... sometimes you want a boost
    filter = update_coefficients<double,double>(filter, 220.0, 0.5, 0.25, 48000.0);

    for(int i = 0; i<BLOCKSIZE; i++){
        input[i] = 0.125*noise<double>();
    }

    std::tie(filter,output) = process<double>(filter, input);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
    
}

TEST(DSP_Test, CORE_resonant_bandpass_amplitude_response) { 
    constexpr size_t FRAME_SIZE = 128;
    constexpr size_t NUM_BINS = 32;
    constexpr double SAMPLE_RATE = 48000;

    reson_bp_t<double> filter;
    filter = update_coefficients<double,double>(filter, 3420.0, 0.5, 0.25, 48000.0);

    std::cout << "a0 = " << 1  << std::endl;
    std::cout << "a1 = " << filter.a1d  << std::endl;
    std::cout << "a2 = " << filter.a2d  << std::endl;
    std::cout << "b0 = " << filter.b0d  << std::endl;
    std::cout << "b1 = " << filter.b1d  << std::endl;
    std::cout << "b2 = " << filter.b2d  << std::endl;
    
    std::array<double, NUM_BINS> amp_response = compute_amplitude_response<double, reson_bp_t<double>, NUM_BINS, FRAME_SIZE>(filter, SAMPLE_RATE);

    //expected values computed in octave using freqz function to evaluate filter response based on coefficients
    std::array<double, NUM_BINS> expected_response = {
        0,
        0.230753381267204,
        0.469155819928966,
        0.706328152678762,
        0.900195945718271,
        0.994229300500133,
        0.980760706083847,
        0.905305196603605,
        0.811846059869397,
        0.721789509635018,
        0.641816043135951,
        0.57256842116928,
        0.512804865620623,
        0.460937326421649,
        0.415514469420179,
        0.375331548867305,
        0.339421306149319,
        0.307014373333521,
        0.277497960277938,
        0.250380813954992,
        0.225265530681472,
        0.201827266260712,
        0.179797506445113,
        0.158951687990906,
        0.139099703430252,
        0.120078553592627,
        0.101746598598966,
        0.083978999628573,
        0.066664047989207,
        0.049700153604457,
        0.032993319311588,
        0.016454965933504
    };
    for(size_t bin_idx=0; bin_idx<NUM_BINS; bin_idx++){
        // std::cout << expected_response[bin_idx] << " , " << amp_response[bin_idx] << std::endl;
        EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx],0.01);
    }
}

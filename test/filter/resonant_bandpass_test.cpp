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

using algae::dsp::core::filter::reson_bp;
TEST(DSP_Test, CORE_resonant_bandpass_amplitude_response) { 
    constexpr size_t FRAME_SIZE = 128;
    constexpr size_t NUM_BINS = 32;
    constexpr double SAMPLE_RATE = 48000;

    reson_bp_t<double> filter = reson_bp<double,double>( 3420.0, 0.5, 1, 48000.0);

    std::cout << "a0 = " << 1  << std::endl;
    std::cout << "a1 = " << filter.a1d  << std::endl;
    std::cout << "a2 = " << filter.a2d  << std::endl;
    std::cout << "b0 = " << filter.b0d  << std::endl;
    std::cout << "b1 = " << filter.b1d  << std::endl;
    std::cout << "b2 = " << filter.b2d  << std::endl;
    
    std::array<double, NUM_BINS> amp_response = compute_amplitude_response<double, reson_bp_t<double>, NUM_BINS>(filter, SAMPLE_RATE);

    //expected values computed in octave using freqz function to evaluate filter response based on coefficients
    std::array<double, NUM_BINS> expected_response = {
        2.19633146247128,
        2.11374826750154,
        1.90320780251077,
        1.64071959156745,
        1.38632300758342,
        1.16728391682754,
        0.988534736440327,
        0.84546251500083,
        0.731101846247929,
        0.638981337598576,
        0.563884337014535,
        0.501832601823901,
        0.449855255264027,
        0.405744287577019,
        0.367853378511164,
        0.334946364948773,
        0.306087214636378,
        0.280561303135178,
        0.257819334915452,
        0.237437391205991,
        0.219088437298083,
        0.202522014218178,
        0.187549821998227,
        0.174035561488107,
        0.161887811908895,
        0.151054931107953,
        0.141521007917291,
        0.133301811641021,
        0.126439560788983,
        0.12099535601983,
        0.11703857686261,
        0.114633677119464
    };
    for(size_t bin_idx=0; bin_idx<NUM_BINS; bin_idx++){
        // std::cout << expected_response[bin_idx] << " , " << amp_response[bin_idx] << std::endl;
        EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx],0.01);
    }
}

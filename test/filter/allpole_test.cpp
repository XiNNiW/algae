#include <gtest/gtest.h>
#include "algae.h"
#include "../frequency_test_utilities.h"

using algae::dsp::core::AudioBlock;
using algae::dsp::core::filter::process;
using algae::dsp::core::oscillator::noise;


using algae::dsp::core::filter::allpole_t;
using algae::dsp::core::filter::allpole;
using algae::dsp::core::filter::process;

TEST(Filter_Test, CORE_allpole_construction) { 
    constexpr size_t ORDER = 4;
    allpole_t<double,ORDER> filter;
    double as[ORDER] = {2,2, 4, 6};

    filter = allpole<double,ORDER>(as);

    for(size_t idx=0; idx<ORDER-1; idx++){
        EXPECT_EQ(filter.as[idx],as[idx+1]/2.0);
    }
    
}

TEST(Filter_Test, CORE_allpole_does_not_explode_provides_stable_poles) { 
    constexpr size_t BLOCKSIZE = 64;
    constexpr size_t ORDER = 3;
    allpole_t<double,ORDER> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;
    double a0 = 1;
    double a1 = -1.25825;
    double a2 = 0.395798;

    double as[ORDER] = {a0,a1,a2};

    filter = allpole<double,ORDER>(as);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        input[i] = noise<double>();
    }

    std::tie(filter,output) = process<double,BLOCKSIZE,ORDER>(filter,input);

    double max_magnitude = 0;
    for(size_t i = 0; i<BLOCKSIZE; i++){
        // std::cout << "output: " << output[i] << ", input: " << input[i] << "\n";
        max_magnitude=abs(output[i])>max_magnitude?abs(output[i]):max_magnitude;
    }

    EXPECT_LT(max_magnitude,5);
    EXPECT_GT(max_magnitude,0);
    
}

TEST(Filter_Test, CORE_allpole_amplitude_response) { 
    constexpr size_t NUM_BINS = 32;
    constexpr double SAMPLE_RATE = 192000;
    constexpr size_t ORDER = 3;
    double a0 = 1;
    double a1 = -1.25825;
    double a2 = 0.395798;

    double as[ORDER] = {a0,a1,a2};
    
    allpole_t<double,ORDER> filter =  allpole<double,ORDER>(as);
    
    std::array<double, NUM_BINS> amp_response = compute_amplitude_response<double, allpole_t<double,ORDER>, NUM_BINS>(filter, SAMPLE_RATE);

    //expected values computed in signal python using freqz function to evaluate filter response based on coefficients
    std::array<double, NUM_BINS> expected_response = {
        7.270189315729787,
        6.963455974963548,
        6.183334152635428,
        5.215720748410942,
        4.285832238161064,
        3.495025055353928,
        2.860396931702335,
        2.3631829203539647,
        1.9759666439869694,
        1.6734352060662772,
        1.4352303202375125,
        1.2458519814992424,
        1.093748381541115,
        0.9703577285859104,
        0.8693187492192285,
        0.7858741654524272,
        0.7164358505455579,
        0.6582721982766534,
        0.6092840105488725,
        0.5678433798486314,
        0.5326771687228805,
        0.5027821066106707,
        0.4773624188821276,
        0.4557836333628147,
        0.4375381050419817,
        0.42221911308416693,
        0.40950129711752237,
        0.3991258381042802,
        0.3908892392639162,
        0.38463488343617663,
        0.3802467752263896,
        0.37764504702024143
    };
    for(size_t bin_idx=0; bin_idx<NUM_BINS; bin_idx++){
        EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx],0.01);
    }
}



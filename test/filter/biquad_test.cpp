#include <gtest/gtest.h>
#include "../../src/shell/oscillator.h"
#include "../../src/shell/filter.h"
#include "../frequency_test_utilities.h"
#include <iostream>

using algae::dsp::core::AudioBlock;
using algae::dsp::core::filter::process;
using algae::dsp::core::oscillator::noise;


using algae::dsp::core::filter::biquad_t;
using algae::dsp::core::filter::lowpass;
using algae::dsp::core::filter::highpass;

TEST(Filter_Test, CORE_biquad_lowpass_does_not_explode) { 
    constexpr size_t BLOCKSIZE = 64;
    biquad_t<double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    filter = lowpass<double,double>(filter,220,0.5,48000);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        input[i] = noise<double>();
    }

    std::tie(filter,output) = process<double>(filter,input);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-2);
        EXPECT_LT(output[i],2);
    }
    
}

TEST(Filter_Test, CORE_biquad_lowpass_does_not_explode_FLOAT) { 
    constexpr size_t BLOCKSIZE = 64;
    biquad_t<float> filter;
    AudioBlock<float,BLOCKSIZE> input;
    AudioBlock<float,BLOCKSIZE> output;

    filter = lowpass<float,float>(filter,220,0.5,48000);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        input[i] = noise<float>();
    }

    std::tie(filter,output) = process<float>(filter,input);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-2);
        EXPECT_LT(output[i],2);
    }
    
}

TEST(Filter_Test, CORE_biquad_lowpass_coefficients) {
    /*
    https://www.earlevel.com/main/2010/12/20/biquad-calculator/
    EXPECTED FOR: cutoff = 220, q=0.5
    b0 = 0.00020151353208571057
    b1 = 0.00040302706417142114
    b2 = 0.00020151353208571057
    a1 = -1.943217815176137
    a2 = 0.9440238693044798
    */
    biquad_t<double> filter;
    filter = lowpass<double,double>(filter, 220, 0.5, 48000);
    double epsilon = 0.000001;
    EXPECT_NEAR( -1.943217815176137, filter.a1,  epsilon);
    EXPECT_NEAR( 0.9440238693044798 , filter.a2,  epsilon);
    EXPECT_NEAR( 0.00020151353208571057 , filter.b0,  epsilon);
    EXPECT_NEAR( 0.00040302706417142114 , filter.b1,  epsilon);
    EXPECT_NEAR( 0.00020151353208571057 , filter.b2,  epsilon);
}

TEST(Filter_Test, CORE_biquad_lowpass_amplitude_response) { 
    constexpr size_t NUM_BINS = 32;
    constexpr double SAMPLE_RATE = 48000;

    biquad_t<double> filter;
    filter = lowpass<double,double>(filter, 1000, 0.5, 48000);
    
    std::array<double, NUM_BINS> amp_response = compute_amplitude_response<double, biquad_t<double>, NUM_BINS>(filter, SAMPLE_RATE);

    //expected values computed in octave using freqz function to evaluate filter response based on coefficients
    std::array<double, NUM_BINS> expected_response = {
        0.999788611441399,
        0.640264937441577,
        0.30693822366928,
        0.163351063962347,
        0.097943931621367,
        0.064081512075196,
        0.04460345660056,
        0.032466375762941,
        0.024427142959851,
        0.018842657595971,
        0.014813768273712,
        0.011816745808295,
        0.009530510691689,
        0.00774966081001,
        0.006337999663849,
        0.005202419866909,
        0.004277579057314,
        0.00351656731614,
        0.002885036302516,
        0.002357399608343,
        0.001914311199764,
        0.001540954481378,
        0.001225858319814,
        0.000960063356335,
        0.000736525979128,
        0.000549686638011,
        0.000395153880667,
        0.000269471340667,
        0.000169945290733,
        0.000094517314107658,
        0.0000416713831490537,
        0.0000103679451089572
    };
    for(size_t bin_idx=0; bin_idx<NUM_BINS; bin_idx++){
        EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx],0.01);
    }
}




using algae::dsp::core::filter::highpass;

TEST(Filter_Test, CORE_biquad_highpass_does_not_explode) { 

    constexpr size_t BLOCKSIZE = 64;
    biquad_t<double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    filter = highpass<double,double>(filter,220,0.5,48000);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        input[i] = noise<double>();
    }

    std::tie(filter,output) = process<double>(filter,input);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-2);
        EXPECT_LT(output[i],2);
    }
    
}

TEST(Filter_Test, CORE_biquad_highpass_amplitude_response) { 
    constexpr size_t NUM_BINS = 32;
    constexpr double SAMPLE_RATE = 48000;

    biquad_t<double> filter;
    filter = highpass<double,double>(filter, 2000, 0.5, 48000);
    
    std::array<double, NUM_BINS> amp_response = compute_amplitude_response<double, biquad_t<double>, NUM_BINS>(filter, SAMPLE_RATE);

    //expected values computed in octave using freqz function to evaluate filter response based on coefficients
    std::array<double, NUM_BINS> expected_response = {
        3.69405810764026E-05,
        0.122186789148331,
        0.358813204211454,
        0.559361515152099,
        0.695376093447049,
        0.783550844008875,
        0.841500097424935,
        0.880762285459868,
        0.908250497076891,
        0.928091881739818,
        0.942805861101011,
        0.95397658160031,
        0.962630754972054,
        0.969453143210067,
        0.974912571929108,
        0.979337701443556,
        0.982963862613871,
        0.985962774842868,
        0.988461866396189,
        0.990557118551635,
        0.992321780285165,
        0.993812391777296,
        0.995073018195025,
        0.996138270592429,
        0.997035490183264,
        0.997786345732512,
        0.998408012479269,
        0.998914047742275,
        0.999315042853078,
        0.999619106959776,
        0.999832221562762,
        0.999958492833479
    };
    for(size_t bin_idx=0; bin_idx<NUM_BINS; bin_idx++){
        EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx],0.01);
    }
}


using algae::dsp::core::filter::bandpass;

TEST(Filter_Test, CORE_biquad_bandpass_does_not_explode) { 

    constexpr size_t BLOCKSIZE = 64;
    biquad_t<double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    filter = bandpass<double,double>(filter,220,0.5,48000);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        input[i] = noise<double>();
    }

    std::tie(filter,output) = process<double>(filter,input);

    for(size_t i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-2);
        EXPECT_LT(output[i],2);
    }
    
}

TEST(Filter_Test, CORE_biquad_bandpass_amplitude_response) { 
    constexpr size_t NUM_BINS = 32;
    constexpr double SAMPLE_RATE = 48000;

    biquad_t<double> filter;
    filter = bandpass<double,double>(filter, 4000, 0.8, 48000);
    
    std::array<double, NUM_BINS> amp_response = compute_amplitude_response<double, biquad_t<double>, NUM_BINS>(filter, SAMPLE_RATE);

    //expected values computed in octave using octave freqz function to evaluate filter response based on coefficients
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
        EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx],0.01);
    }
}



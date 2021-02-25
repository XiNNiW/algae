#include <gtest/gtest.h>
#include "../src/shell/dsp_graph.h"
#include "../src/shell/oscillator.h"
#include "../src/shell/filter.h"
#include <iostream>

using algae::dsp::shell::dsp_node;
using algae::dsp::shell::connect;

TEST(DSP_Test, LowpassFilter) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    dsp_node<double,double>* filter_node = new algae::dsp::shell::filter::lpf<double,double>();

    ASSERT_EQ(0,filter_node->getOutputValue(0));
    filter_node->update();


    dsp_node<double,double>* noise_node = new algae::dsp::shell::oscillator::noise<double,double>();

    connect(noise_node,0,filter_node,0);
    connect(noise_node,0,filter_node,1);
    connect(noise_node,0,filter_node,2);
    filter_node->update();
    bool inRange = -1<=filter_node->getOutputValue(0)<=1;
    ASSERT_TRUE(inRange);

}

TEST(DSP_Test, HighPassFilter) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    dsp_node<double,double>* filter_node = new algae::dsp::shell::filter::hpf<double,double>();

    ASSERT_EQ(0,filter_node->getOutputValue(0));
    filter_node->update();


    dsp_node<double,double>* noise_node = new algae::dsp::shell::oscillator::noise<double,double>();

    connect(noise_node,0,filter_node,0);
    connect(noise_node,0,filter_node,1);
    connect(noise_node,0,filter_node,2);
    filter_node->update();
    bool inRange = -1<=filter_node->getOutputValue(0)<=1;
    ASSERT_TRUE(inRange);

}

TEST(DSP_Test, BandPassFilter) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    dsp_node<double,double>* filter_node = new algae::dsp::shell::filter::bpf<double,double>();

    ASSERT_EQ(0,filter_node->getOutputValue(0));
    filter_node->update();

    dsp_node<double,double>* noise_node =  new algae::dsp::shell::oscillator::noise<double,double>();

    connect(noise_node,0,filter_node,0);
    connect(noise_node,0,filter_node,1);
    connect(noise_node,0,filter_node,2);
    filter_node->update();
    bool inRange = -1<=filter_node->getOutputValue(0)<=1;
    ASSERT_TRUE(inRange);

}


using algae::dsp::core::filter::bandpass_t;
using algae::dsp::core::filter::process_bandpass;
using algae::dsp::core::filter::update_coefficients;
TEST(DSP_Test, CORE_BandPassFilter_Moore_Has_BP_Characteristic) { 
    // TODO: i need a dft function to do this properly
    bandpass_t<double> filter;
    double highest_frequency_signal[10] = {1,-1,1,-1,1,-1,1,-1,1,-1};
    double lowest_frequency_signal[10] = {1,1,1,1,1,1,1,1,1,1};
    double output[10]={};
    double rms;

    double sample_rate = 48000;
    double half_sample_rate = 24000;
    filter = update_coefficients<double,double>(bandpass_t<double>(), half_sample_rate, 0, sample_rate );

    for (int n=0; n<10; n++){
        filter = process_bandpass<double>(filter, highest_frequency_signal[0]);
        output[n] = filter.y1;
        rms += output[n]*output[n];
    }
    
    rms = sqrt(rms)/10.0;

    EXPECT_GT(1, rms);
    EXPECT_LT(0, rms);

    rms = 0;

    filter = update_coefficients<double,double>(bandpass_t<double>(), half_sample_rate, 0, sample_rate );

    for (int n=0; n<10; n++){
        filter = process_bandpass<double>(filter, lowest_frequency_signal[0]);
        output[n] = filter.y1;
        rms += output[n]*output[n];

    }

    rms = sqrt(rms)/10.0;
    
    EXPECT_GT(1, rms);
    EXPECT_LT(0, rms);
    

}





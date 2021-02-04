#include <gtest/gtest.h>
#include <libAlgae_dsp/src/shell/dsp_graph.h>
#include <libAlgae_dsp/src/shell/oscillator.h>
#include <libAlgae_dsp/src/shell/filter.h>
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
    bool inRange = -1<filter_node->getOutputValue(0)<1;
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
    bool inRange = -1<filter_node->getOutputValue(0)<1;
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
    bool inRange = -1<filter_node->getOutputValue(0)<1;
    ASSERT_TRUE(inRange);

}




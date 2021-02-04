#include <gtest/gtest.h>
#include <libAlgae_dsp/src/shell/dsp_graph.h>
#include <libAlgae_dsp/src/shell/oscillator.h>
#include <libAlgae_dsp/src/shell/filter.h>
#include <iostream>

using algae::dsp::shell::dsp_node;
using algae::dsp::shell::connect;

TEST(DSP_Test, RampTest) { 
    
    // dsp_node<double,double>* filter_node = new algae::dsp::shell::control::ramp<double,double>();

    // ASSERT_EQ(0,filter_node->getOutputValue(0));
    // filter_node->update();


    // dsp_node<double,double>* noise_node = new algae::dsp::shell::oscillator::noise<double,double>();

    // connect(noise_node,0,filter_node,0);
    // connect(noise_node,0,filter_node,1);
    // connect(noise_node,0,filter_node,2);
    // filter_node->update();
    // bool inRange = -1<filter_node->getOutputValue(0)<1;
    // ASSERT_TRUE(inRange);

    ASSERT_TRUE(false);
}





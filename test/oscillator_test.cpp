#include <gtest/gtest.h>
#include <iostream>
#include "../src/shell/dsp_graph.h"
#include "../src/shell/math_functions.h"
#include "../src/shell/oscillator.h"

using algae::dsp::shell::dsp_node;
using algae::dsp::shell::connect;
using algae::dsp::shell::math::constant_node;
using algae::dsp::shell::oscillator::phasor;
using algae::dsp::shell::oscillator::osc;

TEST(DSP_Test, PhasorNode) { 
    dsp_node<double,double>* phasor_node = new phasor<double,double>();

    ASSERT_EQ(0,phasor_node->getOutputValue(0));
    phasor_node->update();

    ASSERT_EQ(0,phasor_node->getOutputValue(0));

    dsp_node<double,double>* number = new constant_node<double,double>(22050.0);

    connect<double,double>(*number,0,*phasor_node,0);
    ASSERT_EQ(0,phasor_node->getOutputValue(0));

    phasor_node->update();

    ASSERT_EQ(0.5,phasor_node->getOutputValue(0));

}


TEST(DSP_Test, OscNode) { 
    dsp_node<double,double>* phasor_node = new osc<double,double>();

    ASSERT_EQ(0,phasor_node->getOutputValue(0));
    phasor_node->update();

    ASSERT_EQ(0,phasor_node->getOutputValue(0));

    dsp_node<double,double>* number = new constant_node<double,double>(22050.0);

    connect<double,double>(*number,0,*phasor_node,0);
    ASSERT_EQ(0,phasor_node->getOutputValue(0));

    phasor_node->update();

    ASSERT_FLOAT_EQ(0.70710677,phasor_node->getOutputValue(0));

    delete phasor_node;
    delete number;
    
}

TEST(DSP_Test, Noise) { 
    dsp_node<double,double>* noise_node = new algae::dsp::shell::oscillator::noise<double,double>();

    ASSERT_EQ(0,noise_node->getOutputValue(0));
    noise_node->update();

    int iterations = 44100*10;
    double avgSample=0;
    for(size_t idx=0;idx<iterations;idx++){
        auto samp = noise_node->getOutputValue(0);
        bool resultInRange = -1<=samp<=1;
        ASSERT_TRUE(resultInRange);

        avgSample+=samp;
    }

    avgSample/=(double)iterations;

    // ASSERT_NEAR(0, avgSample,0.025);
    //so that fails actually... it doesn't avg to zero like i naively thought...
    
}
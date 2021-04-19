#include <gtest/gtest.h>
#include <iostream>
#include "../src/shell/dsp_graph.h"
#include "../src/shell/math_functions.h"
#include "../src/shell/oscillator.h"
#include "../src/core/oscillator.h"

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

        avgSample += samp*samp;
    }

    avgSample /=(double)iterations;
    avgSample = sqrt(avgSample);

    // ASSERT_NEAR(0, avgSample,0.025);
    //so that fails actually... it doesn't avg to zero like i naively thought...
    
}

using algae::dsp::core::oscillator::phasor_t;
using algae::dsp::core::oscillator::update_phasor;
using algae::dsp::core::oscillator::dsf_bl_pulse;

TEST(DSP_Test, CORE_dsf_blp_does_not_explode) { 
    phasor_t<double> phasor = phasor_t<double>();
    phasor_t<double> phasor2 = phasor_t<double>();
    const int iterations = 100;
    double output[iterations] = {};
    double expected_output[iterations] = {};
    double freq = 10;
    double sampleRate = 1000;

    for(int n=0;n<iterations;n++){
        double output = dsf_bl_pulse<double>(phasor.phase, phasor2.phase, sampleRate);
        EXPECT_LT(output,1);
        EXPECT_GT(output,-1);
        phasor = update_phasor<double,double>(phasor,freq);
    }
}

using algae::dsp::core::oscillator::Bn;
TEST(DSP_Test, CORE_Bn_bandlimited_component_generator) { 
    
    EXPECT_FLOAT_EQ(Bn(5,7.0*M_PI),5.0);
    EXPECT_FLOAT_EQ(Bn(8,7.0*M_PI),-8.0);
    EXPECT_FLOAT_EQ(Bn(8,6.0*M_PI),8.0);
    EXPECT_FLOAT_EQ(Bn(8,6.01*M_PI),-7.9792895);
}


using algae::dsp::core::oscillator::fm_bl_saw_t;
using algae::dsp::core::oscillator::dc_block_t;
using algae::dsp::core::oscillator::fm_bl_saw;
using algae::dsp::core::oscillator::process_fm_bl_saw;
TEST(DSP_Test, CORE_fm_blp_saw) { 
    phasor_t<double> phasor = phasor_t<double>();
    double freq = 100;
    double index = 10;
    double sampleRate = 1000;

    fm_bl_saw_t<double,double> state = fm_bl_saw<double,double>(fm_bl_saw_t<double,double>(),freq,sampleRate);
    const int iterations = 100;
    double output[iterations] = {};
    double expected_output[iterations] = {};
    
    for(int n=0;n<iterations;n++){
        state = process_fm_bl_saw<double>(state, sampleRate);
        double output = state.output;
        EXPECT_LT(output,1.0001);
        EXPECT_GT(output,-1.0001);
    }
}

using algae::dsp::core::oscillator::fm_bl_square_t;
using algae::dsp::core::oscillator::fm_bl_square;
using algae::dsp::core::oscillator::process_fm_bl_square;
TEST(DSP_Test, CORE_fm_blp_square) { 
    phasor_t<double> phasor = phasor_t<double>();
    double freq = 100;
    double index = 10;
    double sampleRate = 1000;

    fm_bl_square_t<double,double> state = fm_bl_square<double,double>(fm_bl_square_t<double,double>(),freq, sampleRate);
    const int iterations = 100;
    double output[iterations] = {};
    double expected_output[iterations] = {};
    
    for(int n=0;n<iterations;n++){
        state = process_fm_bl_square<double>(state, sampleRate);
        double output = state.output;

        EXPECT_LT(output,1);
        EXPECT_GT(output,-1);
    }
}

using algae::dsp::core::oscillator::fm_bl_tri_t;
using algae::dsp::core::oscillator::fm_bl_tri;
using algae::dsp::core::oscillator::process_fm_bl_tri;
TEST(DSP_Test, CORE_fm_blp_tri) { 
    phasor_t<double> phasor = phasor_t<double>();
    double freq = 100;
    double sampleRate = 1000;
    fm_bl_tri_t<double,double> state = fm_bl_tri<double,double>(fm_bl_tri_t<double,double>(),freq,sampleRate);
    const int iterations = 100;
    double output[iterations] = {};
    double expected_output[iterations] = {};
    
    for(int n=0;n<iterations;n++){
        state = process_fm_bl_tri<double>(state, sampleRate);
        double output = state.output;

        EXPECT_LT(output,1);
        EXPECT_GT(output,-1);
    }
}

//max_bl_modulation_index
using algae::dsp::core::oscillator::max_bl_modulation_index;
TEST(DSP_Test, CORE_max_bl_modulation_index) { 
    EXPECT_FLOAT_EQ(108.09091,max_bl_modulation_index<double>(220,220,48000));
    EXPECT_FLOAT_EQ(53.545456,max_bl_modulation_index<double>(440,440,48000));
    EXPECT_FLOAT_EQ(26.772728,max_bl_modulation_index<double>(440,880,48000));
    EXPECT_FLOAT_EQ(54.545456,max_bl_modulation_index<double>(0,440,48000));
    EXPECT_FLOAT_EQ(0,max_bl_modulation_index<double>(440,0,48000));
}

using algae::dsp::core::oscillator::makeSineTable;
const std::array<double, 4> sine_4 = makeSineTable<double, 4>(); 
const std::array<double, 4> expected_sine_4 = {0,1,.00000000000000012246469,-1}; 
//sine wavetable
TEST(DSP_Test, CORE_makeSineWave) {

    size_t idx=0;
    double val;
    double expected;
    val = sine_4[idx];
    expected = expected_sine_4[idx++];
    EXPECT_FLOAT_EQ(expected, val);
    idx++;
    val = sine_4[idx];
    expected = expected_sine_4[idx++];
    EXPECT_FLOAT_EQ(expected, val);
    idx++;
    val = sine_4[idx];
    expected = expected_sine_4[idx++];
    EXPECT_FLOAT_EQ(expected, val);
    idx++;
    val = sine_4[idx];
    expected = expected_sine_4[idx++];
    EXPECT_FLOAT_EQ(expected, val);
    idx++;

}

using algae::dsp::core::oscillator::table_lookup_lin_interp;

TEST(DSP_Test, CORE_lookupSine_linear_interp) {
    const std::array<double, 4> table = makeSineTable<double, 4>(); 
    size_t idx=0;
    double val;
    double expected;
    val = table_lookup_lin_interp<double,4>(table.data(), idx/4.0);
    expected = expected_sine_4[idx];
    EXPECT_FLOAT_EQ(expected, val);
    idx++;
    val = table_lookup_lin_interp<double,4>(table.data(), idx/4.0);
    expected = expected_sine_4[idx];
    EXPECT_FLOAT_EQ(expected, val);
    val = table_lookup_lin_interp<double,4>(table.data(), idx/4.0);
    expected = expected_sine_4[idx];
    EXPECT_FLOAT_EQ(expected, val);
    val = table_lookup_lin_interp<double,4>(table.data(), idx/4.0);
    expected = expected_sine_4[idx];
    EXPECT_FLOAT_EQ(expected, val);


}
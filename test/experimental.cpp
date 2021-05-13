#include <gtest/gtest.h>
#include <iostream>
#include "../src/shell/dsp_graph.h"
#include "../src/shell/math_functions.h"
#include "../src/shell/oscillator.h"
#include "../src/core/oscillator.h"
#include "../src/core/experimental.h"

using algae::dsp::shell::dsp_node;
using algae::dsp::shell::connect;
using algae::dsp::shell::math::constant_node;
using algae::dsp::shell::oscillator::phasor;
using algae::dsp::shell::oscillator::osc;
using algae::dsp::core::experimental::dsf_bl_pulse;

using algae::dsp::core::filter::integrator_t;
using algae::dsp::core::filter::process_integrator;
TEST(DSP_Test, CORE_integrator_accumulates_signal) { 
    double accum;
    integrator_t<double> filter = integrator_t<double>();
    double input[10] = {1,1,1,0,0,0,0,0,0,0};
    double expected[10] = {1,2,3,3,3,3,3,3,3,3};
    double output[10] = {};

    for(int i=0; i<10; i++){
        filter = process_integrator(filter,input[i]);
        output[i] = filter.y1;
        EXPECT_FLOAT_EQ(expected[i], output[i]);
    }

}

using algae::dsp::core::filter::leaky_integrator_t;
using algae::dsp::core::filter::leaky_integrator;
using algae::dsp::core::filter::process_leaky_integrator;
TEST(DSP_Test, CORE_leaky_integrator_accumulates_signal_above_cutoffhz) { 
    double accum;
    double cutoff= 20.0;
    double sampleRate= 48000;
    leaky_integrator_t<double> filter = leaky_integrator<double,double>(leaky_integrator_t<double>(),cutoff,sampleRate);
    double input[10] = {1,1,1,0,0,0,0,0,0,0};
    double expected[10] = {0,0,0,0,0,0,0,0,0,0};
    double output[10] = {};

    for(int i=0; i<10; i++){
        filter = process_leaky_integrator(filter,input[i]);
        output[i] = filter.y1;
        EXPECT_FLOAT_EQ(expected[i], output[i]);
    }

}
// TEST(DSP_Test, LowpassFilter) { 
//     // what is a compelling way to test these? i should probably just sit down and do the math
//     // in the mean time i just want some verification that they don't blow up when you hook them up
//     dsp_node<double,double>* filter_node = new algae::dsp::shell::filter::lpf<double,double>();

//     ASSERT_EQ(0,filter_node->getOutputValue(0));
//     filter_node->update();


//     dsp_node<double,double>* noise_node = new algae::dsp::shell::oscillator::noise<double,double>();

//     connect(noise_node,0,filter_node,0);
//     connect(noise_node,0,filter_node,1);
//     connect(noise_node,0,filter_node,2);
//     filter_node->update();
//     bool inRange = -1<=filter_node->getOutputValue(0) && filter_node->getOutputValue(0)<=1;
//     ASSERT_TRUE(inRange);

// }
TEST(DSP_Test, PhasorNode) { 
//     dsp_node<double,double>* phasor_node = new phasor<double,double>();

//     ASSERT_EQ(0,phasor_node->getOutputValue(0));
//     phasor_node->update();

//     ASSERT_EQ(0,phasor_node->getOutputValue(0));

//     dsp_node<double,double>* number = new constant_node<double,double>(22050.0);

//     connect<double,double>(*number,0,*phasor_node,0);
//     ASSERT_EQ(0,phasor_node->getOutputValue(0));

//     phasor_node->update();

//     ASSERT_EQ(0.5,phasor_node->getOutputValue(0));

// }


// TEST(DSP_Test, OscNode) { 
//     dsp_node<double,double>* phasor_node = new osc<double,double>();

//     ASSERT_EQ(0,phasor_node->getOutputValue(0));
//     phasor_node->update();

//     ASSERT_EQ(0,phasor_node->getOutputValue(0));

//     dsp_node<double,double>* number = new constant_node<double,double>(22050.0);

//     connect<double,double>(*number,0,*phasor_node,0);
//     ASSERT_EQ(0,phasor_node->getOutputValue(0));

//     phasor_node->update();

//     ASSERT_FLOAT_EQ(0.70710677,phasor_node->getOutputValue(0));

//     delete phasor_node;
//     delete number;
    
// }

// TEST(DSP_Test, Noise) { 
//     dsp_node<double,double>* noise_node = new algae::dsp::shell::oscillator::noise<double,double>();

//     ASSERT_EQ(0,noise_node->getOutputValue(0));
//     noise_node->update();

//     int iterations = 44100*10;
//     double avgSample=0;
//     for(size_t idx=0;idx<iterations;idx++){
//         auto samp = noise_node->getOutputValue(0);
//         bool resultInRange = (-1<=samp) && (samp<=1);
//         ASSERT_TRUE(resultInRange);

//         avgSample += samp*samp;
//     }

//     avgSample /=(double)iterations;
//     avgSample = sqrt(avgSample);

//     // ASSERT_NEAR(0, avgSample,0.025);
//     //so that fails actually... it doesn't avg to zero like i naively thought...
    
// }
using algae::dsp::core::filter::moog_t;
using algae::dsp::core::filter::setFilterParameters;
TEST(DSP_Test, CORE_MoogFilter) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    moog_t<double,double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    filter = setFilterParameters<double,double>(filter,220,0.5,48000);

    for(int i = 0; i<BLOCKSIZE; i++){
        input[i] = noise<double>();
    }

    std::tie(filter,output) = process<double,double>(filter,input,48000);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
    
}

using algae::dsp::core::filter::notch;

TEST(DSP_Test, CORE_biquad_notch) { 
    // what is a compelling way to test these? i should probably just sit down and do the math
    // in the mean time i just want some verification that they don't blow up when you hook them up
    constexpr size_t BLOCKSIZE = 64;
    biquad_t<double> filter;
    AudioBlock<double,BLOCKSIZE> input;
    AudioBlock<double,BLOCKSIZE> output;

    filter = notch<double,double>(filter,220,0.5,48000);

    for(int i = 0; i<BLOCKSIZE; i++){
        input[i] = noise<double>();
    }

    std::tie(filter,output) = process<double,double>(filter,input);

    for(int i = 0; i<BLOCKSIZE; i++){
        EXPECT_GT(output[i],-1.0001);
        EXPECT_LT(output[i],1.0001);
    }
    
}


// TEST(DSP_Test, CORE_dsf_blp_does_not_explode) { 
//     phasor_t<double> phasor = phasor_t<double>();
//     phasor_t<double> phasor2 = phasor_t<double>();
//     const int iterations = 100;
//     double output[iterations] = {};
//     double expected_output[iterations] = {};
//     double freq = 10;
//     double sampleRate = 1000;

//     for(int n=0;n<iterations;n++){
//         double output = dsf_bl_pulse<double>(phasor.phase, phasor2.phase, sampleRate);
//         EXPECT_LT(output,1);
//         EXPECT_GT(output,-1);
//         phasor = update_phasor<double,double>(phasor,freq);
//     }
// }

// using algae::dsp::core::oscillator::Bn;
// TEST(DSP_Test, CORE_Bn_bandlimited_component_generator) { 
    
//     EXPECT_FLOAT_EQ(Bn(5,7.0*M_PI),5.0);
//     EXPECT_FLOAT_EQ(Bn(8,7.0*M_PI),-8.0);
//     EXPECT_FLOAT_EQ(Bn(8,6.0*M_PI),8.0);
//     EXPECT_FLOAT_EQ(Bn(8,6.01*M_PI),-7.9792895);
// }


// using algae::dsp::core::oscillator::fm_bl_saw_t;
// using algae::dsp::core::oscillator::dc_block_t;
// using algae::dsp::core::oscillator::fm_bl_saw;
// using algae::dsp::core::oscillator::process_fm_bl_saw;
// TEST(DSP_Test, CORE_fm_blp_saw) { 
//     phasor_t<double> phasor = phasor_t<double>();
//     double freq = 100;
//     double index = 10;
//     double sampleRate = 1000;

//     fm_bl_saw_t<double,double> state = fm_bl_saw<double,double>(fm_bl_saw_t<double,double>(),freq,sampleRate);
//     const int iterations = 100;
//     double output[iterations] = {};
//     double expected_output[iterations] = {};
    
//     for(int n=0;n<iterations;n++){
//         state = process_fm_bl_saw<double>(state, sampleRate);
//         double output = state.output;
//         EXPECT_LT(output,1.0001);
//         EXPECT_GT(output,-1.0001);
//     }
// }

// using algae::dsp::core::oscillator::fm_bl_square_t;
// using algae::dsp::core::oscillator::fm_bl_square;
// using algae::dsp::core::oscillator::process_fm_bl_square;
// TEST(DSP_Test, CORE_fm_blp_square) { 
//     phasor_t<double> phasor = phasor_t<double>();
//     double freq = 100;
//     double index = 10;
//     double sampleRate = 1000;

//     fm_bl_square_t<double,double> state = fm_bl_square<double,double>(fm_bl_square_t<double,double>(),freq, sampleRate);
//     const int iterations = 100;
//     double output[iterations] = {};
//     double expected_output[iterations] = {};
    
//     for(int n=0;n<iterations;n++){
//         state = process_fm_bl_square<double>(state, sampleRate);
//         double output = state.output;

//         EXPECT_LT(output,1);
//         EXPECT_GT(output,-1);
//     }
// }

// using algae::dsp::core::oscillator::fm_bl_tri_t;
// using algae::dsp::core::oscillator::fm_bl_tri;
// using algae::dsp::core::oscillator::process_fm_bl_tri;
// TEST(DSP_Test, CORE_fm_blp_tri) { 
//     phasor_t<double> phasor = phasor_t<double>();
//     double freq = 100;
//     double sampleRate = 1000;
//     fm_bl_tri_t<double,double> state = fm_bl_tri<double,double>(fm_bl_tri_t<double,double>(),freq,sampleRate);
//     const int iterations = 100;
//     double output[iterations] = {};
//     double expected_output[iterations] = {};
    
//     for(int n=0;n<iterations;n++){
//         state = process_fm_bl_tri<double>(state, sampleRate);
//         double output = state.output;

//         EXPECT_LT(output,1);
//         EXPECT_GT(output,-1);
//     }
// }

// //max_bl_modulation_index
// using algae::dsp::core::oscillator::max_bl_modulation_index;
// TEST(DSP_Test, CORE_max_bl_modulation_index) { 
//     EXPECT_FLOAT_EQ(108.09091,max_bl_modulation_index<double>(220,220,48000));
//     EXPECT_FLOAT_EQ(53.545456,max_bl_modulation_index<double>(440,440,48000));
//     EXPECT_FLOAT_EQ(26.772728,max_bl_modulation_index<double>(440,880,48000));
//     EXPECT_FLOAT_EQ(54.545456,max_bl_modulation_index<double>(0,440,48000));
//     EXPECT_FLOAT_EQ(0,max_bl_modulation_index<double>(440,0,48000));
// }

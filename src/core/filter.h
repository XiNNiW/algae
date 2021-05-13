#pragma once
#include <math.h>
#include "audio_block.h"
#include "math_functions.h"

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::filter{
// http://musicdsp.org/files/Audio-EQ-Cookbook.txt

    using algae::dsp::core::math::clamp;

    constexpr auto HALF_PI = M_PI_2; 
    constexpr auto TWO_PI = 2*M_PI; 

    template<typename sample_t>
    struct biquad_t {
        sample_t b0, b1, b2, a1, a2;
        sample_t y1=0;
        sample_t y2=0;
        sample_t x1=0;
        sample_t x2=0;
    };

    template<typename sample_t, typename frequency_t>
    const inline biquad_t<sample_t> lowpass(
        biquad_t<sample_t> state,
        const frequency_t& cutoff,
        sample_t q,
        const frequency_t& sampleRate=44100.0 
    ) {
        q = q<=0?0.01:q;
        const frequency_t w0 = cutoff * TWO_PI / sampleRate;
        const frequency_t a = fabs(sin(w0) / (2 * q));
        const frequency_t c = cos(w0);
        const frequency_t b0 = (1 - c) / 2, b1 = 1 - c, b2 = (1 - c) / 2;
        const frequency_t a0 = 1 + a, a1 = -2 * c, a2 = 1 - a;

        state.b0 = b0 / a0;
        state.b1 = b1 / a0;
        state.b2 = b2 / a0;
        state.a1 = a1 / a0;
        state.a2 = a2 / a0;

        return state;
    }


    template<typename sample_t, typename frequency_t>
    const inline biquad_t<sample_t> process(biquad_t<sample_t> state, const sample_t& input){

        const sample_t b0 = state.b0;
        const sample_t b1 = state.b1;
        const sample_t b2 = state.b2;
        const sample_t a1 = state.a1;
        const sample_t a2 = state.a2;
        const sample_t x0 = input;
        const sample_t x1 = state.x1;
        const sample_t x2 = state.x2;
        const sample_t y1 = state.y1;
        const sample_t y2 = state.y2;
        const sample_t y0 = b0 * x0 + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
        state.y1 = y0;
        state.y2 = y1;
        state.x1 = x0;
        state.x2 = x1;

        return state;
    }

    template<typename sample_t, typename frequency_t, size_t BLOCK_SIZE>
    const inline std::pair<biquad_t<sample_t>, AudioBlock<sample_t, BLOCK_SIZE>> process(biquad_t<sample_t> state, const AudioBlock<sample_t,BLOCK_SIZE>& input){


        AudioBlock<sample_t, BLOCK_SIZE> output;
        for(size_t idx=0; idx<BLOCK_SIZE; idx++){
            state = process<sample_t,frequency_t>(state,input[idx]);

            output[idx] = state.y1;
        }
        

        return std::pair(state,output);
    }


    template<typename sample_t, typename frequency_t>
    const inline biquad_t<sample_t> highpass(
        biquad_t<sample_t> state,
        const sample_t& cutoff,
        const sample_t& q,
        const frequency_t& sampleRate
    ){
        frequency_t w0 = cutoff * TWO_PI / sampleRate;
        frequency_t a = fabs(sin(w0)/(2*q));
        frequency_t c = cos(w0);
        frequency_t b0 = (1 + c) / 2;
        frequency_t b1 = -(1 + c);
        frequency_t b2 = (1 + c) / 2;
        frequency_t a0 = 1 + a;
        frequency_t a1 = -2 * c;
        frequency_t a2 = 1 - a;
        
        state.b0 = b0 / a0;
        state.b1 = b1 / a0;
        state.b2 = b2 / a0;
        state.a1 = a1 / a0;
        state.a2 = a2 / a0;

        return state;
    }

    template<typename sample_t, typename frequency_t>
    const inline biquad_t<sample_t> bandpass(
        biquad_t<sample_t> state,
        const sample_t& cutoff,
        const sample_t& quality,
        const frequency_t& sampleRate
    ){
        sample_t q = quality>0?quality:0.01;
        frequency_t w0 = cutoff * TWO_PI / sampleRate;
        frequency_t a = fabs(sin(w0) / (2 * q));
        frequency_t c = cos(w0);
        frequency_t b0 = a;
        frequency_t b1 = 0;
        frequency_t b2 = -a;
        frequency_t a0 = 1 + a;
        frequency_t a1 = -2 * c;
        frequency_t a2 = 1 - a;
        state.b0 = b0 / a0;
        state.b1 = b1 / a0;
        state.b2 = b2 / a0;
        state.a1 = a1 / a0;
        state.a2 = a2 / a0;
        
        return state;
    }

    template<typename sample_t>
    struct onepole_lop_t {
        sample_t a1, b0;
        sample_t x1, y1=0;
    };

    template<typename sample_t,typename frequency_t>
    const inline onepole_lop_t<sample_t> lop(
        onepole_lop_t<sample_t> state, 
        const sample_t& cutoff, 
        const frequency_t& sampleRate=44100.0
    ){
        const frequency_t w = TWO_PI * clamp<sample_t>(fabs(cutoff / sampleRate), 0, 0.5);
        const frequency_t a = (1 - sin(w)) / cos(w);
        const frequency_t b = (1 - a) / 2;

        state.a1 = a;
        state.b0 = b;
        
        return state;
    }

    template<typename sample_t,typename frequency_t>
    const inline onepole_lop_t<sample_t> process(
        onepole_lop_t<sample_t> state, 
        const sample_t& input, 
        const frequency_t& sampleRate=44100.0
    ){
        const sample_t a1 = state.a1;
        const sample_t b0 = state.b0;
        const sample_t x0 = input;
        const sample_t x1 = state.x1;
        const sample_t y1 = state.y1;
        const sample_t y = b0*(x0 + x1) + a1 * y1;
        state.y1 = y;
        state.x1 = x0;

        return state;
    }

    template<typename sample_t,typename frequency_t, size_t BLOCK_SIZE>
    const inline std::pair<onepole_lop_t<sample_t>, AudioBlock<sample_t,BLOCK_SIZE>> process(
        onepole_lop_t<sample_t> state, 
        const AudioBlock<sample_t,BLOCK_SIZE>& input
    ){
        const sample_t a1 = state.a1;
        const sample_t b0 = state.b0;
        
        AudioBlock<sample_t, BLOCK_SIZE> output;
        for(size_t idx=0; idx<BLOCK_SIZE; idx++){

            state = process<sample_t,frequency_t>(state,input[idx]);
            output[idx] = state.y1;
        }

        return std::pair(state,output);
    }


    template<typename sample_t>
    struct onepole_hip_t {
        sample_t a1, b0;
        sample_t x1, y1=0;
    };

    template<typename sample_t,typename frequency_t>
    const inline onepole_hip_t<sample_t> hip(
        onepole_hip_t<sample_t> state, 
        const frequency_t& cutoff,
        const frequency_t& sampleRate=44100.0
    ){

        const frequency_t w = TWO_PI * clamp<sample_t>(fabs(cutoff / sampleRate), 0, sample_t(0.5));
        const frequency_t a = (1 - sin(w)) / cos(w);
        const frequency_t b = (1 + a) / 2;

        state.a1 = a;
        state.b0 = b;

        return state;
    }

    template<typename sample_t,typename frequency_t>
    const inline onepole_hip_t<sample_t> process(
        onepole_hip_t<sample_t> state, 
        const sample_t& input, 
        const frequency_t& sampleRate=44100.0
    ){
        const sample_t a1 = state.a1;
        const sample_t b0 = state.b0;
        const sample_t x0 = input;
        const sample_t x1 = state.x1;
        const sample_t y1 = state.y1;
        const sample_t y = b0*(x0 - x1) + a1 * y1;
        state.y1 = y;
        state.x1 = x0;

        return state;
    }

    template<typename sample_t,typename frequency_t, size_t BLOCK_SIZE>
    const inline std::pair<onepole_hip_t<sample_t>, AudioBlock<sample_t,BLOCK_SIZE>> process(
        onepole_hip_t<sample_t> state, 
        const AudioBlock<sample_t,BLOCK_SIZE>& input
    ){

        AudioBlock<sample_t, BLOCK_SIZE> output;
        for(size_t idx=0; idx<BLOCK_SIZE; idx++){

            state = process<sample_t,frequency_t>(state, input[idx]);
            output[idx] = state.y1;
        }

        return std::pair(state,output);
    }

    template<typename sample_t>
    struct reson_bp_t{
        sample_t b0d;
        sample_t b1d;
        sample_t b2d; 
        sample_t a1d;
        sample_t a2d;
        sample_t x1=0;
        sample_t x2=0;
        sample_t y1=0;
        sample_t y2=0;
    };

    template<typename sample_t, typename frequency_t>
    const inline reson_bp_t<sample_t> update_coefficients(
        reson_bp_t<sample_t> filter, 
        const frequency_t& freq, 
        sample_t q, 
        const sample_t& gain, 
        const frequency_t& sampleRate=44100
    ){
        q = q>0?q:0.01;
        sample_t w = TWO_PI*freq;
        sample_t a1 = 1.0/q;
        sample_t a0 = 1.0;
        sample_t b2 = 0.0;
        sample_t b1 = gain;
        sample_t b0 = 0.0;
        sample_t c = 1.0/tan(0.5*w/sampleRate);
        sample_t csq = c*c;
        sample_t d = a0 + a1*c + csq;

        filter.b0d = (b0 + b1*c + b2*csq)/d;
        filter.b1d = 2*(b0 - b1*c + b2*csq)/d;
        filter.b2d = (b0 - b1*c + b2*csq)/d;
        filter.a1d =  2*(a0 - csq)/d;
        filter.a2d =(a0 - a1*c + csq)/d;

        return filter;
    }

    template<typename sample_t, typename frequency_t>
    const inline reson_bp_t<sample_t> reson_bp(
        const frequency_t& freq, 
        const sample_t& q, 
        const sample_t& gain,
        const frequency_t& sampleRate
    ){
        reson_bp_t<sample_t> filter;

        return update_coefficients(filter, freq, q, gain, sampleRate); 
    }

    template<typename sample_t, typename frequency_t>
    const inline reson_bp_t<sample_t> process(reson_bp_t<sample_t> r, const sample_t& input){
  
        sample_t xn = input;
        sample_t yn = r.b0d*xn + r.b1d*r.x1 + r.b2d*r.x2 - r.a1d*r.y1 - r.a2d*r.y2;

        r.y2 = r.y1;
        r.y1 = yn;
        r.x2 = r.x1;
        r.x1 = xn;
  
        return r;
    }

    template<typename sample_t, typename frequency_t, size_t BLOCK_SIZE>
    const inline std::pair<reson_bp_t<sample_t>, AudioBlock<sample_t,BLOCK_SIZE>> process(reson_bp_t<sample_t> r, const AudioBlock<sample_t, BLOCK_SIZE>& input){
        
        AudioBlock<sample_t,BLOCK_SIZE> output;
        for(size_t idx=0; idx<BLOCK_SIZE; idx++){
            r = process<sample_t,frequency_t>(r, input[idx]);
            output[idx] = r.y1;
        }
        
        return std::pair(r,output);
    }
  

}

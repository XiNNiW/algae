#pragma once
#include <tuple>
#include <math.h>
#include "../constants.h"
namespace algae::dsp::core::filter{
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

    template<typename sample_t>
    const inline reson_bp_t<sample_t> process(reson_bp_t<sample_t> r, const sample_t& input){
  
        sample_t xn = input;
        sample_t yn = r.b0d*xn + r.b1d*r.x1 + r.b2d*r.x2 - r.a1d*r.y1 - r.a2d*r.y2;

        r.y2 = r.y1;
        r.y1 = yn;
        r.x2 = r.x1;
        r.x1 = xn;
  
        return r;
    }

    template<typename sample_t, size_t BLOCK_SIZE>
    const inline std::pair<reson_bp_t<sample_t>, AudioBlock<sample_t,BLOCK_SIZE>> process(reson_bp_t<sample_t> r, const AudioBlock<sample_t, BLOCK_SIZE>& input){
        
        AudioBlock<sample_t,BLOCK_SIZE> output;
        for(size_t idx=0; idx<BLOCK_SIZE; idx++){
            r = process<sample_t>(r, input[idx]);
            output[idx] = r.y1;
        }
        
        return std::pair(r,output);
    }

}
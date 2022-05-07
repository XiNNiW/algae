#pragma once
#include "../audio_block.h"
#include "../constants.h"

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::filter {
    // http://musicdsp.org/files/Audio-EQ-Cookbook.txt
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
        const frequency_t a  = fabs(sin(w0) / (2 * q));
        const frequency_t c  = cos(w0);
        const frequency_t b0 = (1 - c) / 2, b1 = 1 - c, b2 = (1 - c) / 2;
        const frequency_t a0 = 1 + a, a1 = -2 * c, a2 = 1 - a;

        state.b0 = b0 / a0;
        state.b1 = b1 / a0;
        state.b2 = b2 / a0;
        state.a1 = a1 / a0;
        state.a2 = a2 / a0;

        return state;
    }


    template<typename sample_t>
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

    template<typename sample_t, size_t BLOCK_SIZE>
    const inline std::pair<biquad_t<sample_t>, AudioBlock<sample_t, BLOCK_SIZE>> process(biquad_t<sample_t> state, const AudioBlock<sample_t,BLOCK_SIZE>& input){


        AudioBlock<sample_t, BLOCK_SIZE> output;
        for(size_t idx=0; idx<BLOCK_SIZE; idx++){
            state = process<sample_t>(state,input[idx]);

            output[idx] = state.y1;
        }
        

        return std::pair(state,output);
    }

    // template<typename sample_t, typename frequency_t, size_t MAX_UNISON, size_t BLOCK_SIZE>
    // const inline std::pair<biquad_t<sample_t>, std::array<AudioBlock<sample_t, BLOCK_SIZE>, MAX_UNISON>> process(std::array<biquad_t<sample_t>,MAX_UNISON>state, const std::array<AudioBlock<sample_t,BLOCK_SIZE>, MAX_UNISON>& input, const size_t& unison){
    //     size_t _unison = unison>MAX_UNISON?MAX_UNISON:unison;
    //     std::array<AudioBlock<sample_t, BLOCK_SIZE>,MAX_UNISON> output;
    //     for(size_t unison_idx=0; unison_idx<_unison; unison_idx++){
    //         std::tie(state[unison_idx],output[unison_idx]) = process<sample_t,BLOCK_SIZE>(state[unison_idx],output[unison_idx]);
    //     }
    //     return std::pair(state,output);
    // }


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
        //sample_t freq = cutoff>0?cutoff:0.01;
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
}
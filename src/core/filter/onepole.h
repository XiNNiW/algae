#pragma once
#include "../audio_block.h"
#include "../math_functions.h"
#include "../constants.h"

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::filter {
    using algae::dsp::core::math::clamp;

    
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

}
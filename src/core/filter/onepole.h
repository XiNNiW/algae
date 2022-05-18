#pragma once
#include "../audio_block.h"
#include "../math_tools.h"
#include "../constants.h"

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::filter {
    using algae::dsp::core::math::clamp;

    
    template<typename sample_t>
    struct onepole_t {
        sample_t a0=0;
        bool mode = true;
        sample_t y1=0;
    };

    template<typename sample_t,typename frequency_t>
    const inline onepole_t<sample_t> lop(
        onepole_t<sample_t> state, 
        const sample_t& cutoff, 
        const frequency_t& sampleRate
    ){
        sample_t w = M_PI*clamp<sample_t>(fabs(cutoff/sampleRate),0,1);
        state.a0 = sin(w);
        state.mode = true;
        return state;
    }

    template<typename sample_t,typename frequency_t>
    const inline onepole_t<sample_t> hip(
        onepole_t<sample_t> state, 
        const sample_t& cutoff, 
        const frequency_t& sampleRate
    ){
        sample_t w = M_PI*clamp<sample_t>(fabs(-cutoff/sampleRate),0,1);
        state.a0 = sin(w);
        state.mode = false;
        return state;
    }

    template<typename sample_t>
    const inline onepole_t<sample_t> process(
        onepole_t<sample_t> f, 
        const sample_t& input
    ){
        const sample_t xn = input;
        const sample_t a0 = f.a0;
        
        f.y1 = f.mode
            ?a0*(xn - f.y1) + f.y1
            :a0*(xn + f.y1) - f.y1;
        return f;
    }

    template<typename sample_t, size_t BLOCK_SIZE>
    const inline std::pair<onepole_t<sample_t>, AudioBlock<sample_t,BLOCK_SIZE>> process(
        onepole_t<sample_t> state, 
        const AudioBlock<sample_t,BLOCK_SIZE>& input
    ){
        
        AudioBlock<sample_t, BLOCK_SIZE> output;
        for(size_t idx=0; idx<BLOCK_SIZE; idx++){

            state = process<sample_t>(state,input[idx]);
            output[idx] = state.y1;
        }

        return std::pair(state,output);
    }

    template<typename sample_t>
    struct hip_t {
        sample_t a0=0;
        sample_t y1=0;
    };

    template<typename sample_t,typename frequency_t>
    const inline hip_t<sample_t> hip(
        hip_t<sample_t> state, 
        const sample_t& cutoff, 
        const frequency_t& sampleRate
    ){
        sample_t w = M_PI*clamp<sample_t>(fabs(-cutoff/sampleRate),0,1);
        state.a0 = sin(w);
        return state;
    }

    template<typename sample_t>
    const inline std::pair<hip_t<sample_t>, sample_t> process(
        hip_t<sample_t> f, 
        const sample_t& input
    ){
        const sample_t xn = input;
        const sample_t a0 = f.a0;
        
        f.y1 = a0*(xn + f.y1) - f.y1;
        return std::pair(f, f.y1);
    }


    template<typename sample_t>
    struct onepole_onezero_t {
        sample_t a1=0, b0=0;
        sample_t x1=0, y1=0;
    };

    template<typename sample_t,typename frequency_t>
    const inline onepole_onezero_t<sample_t> hip(
        onepole_onezero_t<sample_t> state, 
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

    template<typename sample_t>
    const inline onepole_onezero_t<sample_t> process(
        onepole_onezero_t<sample_t> state, 
        const sample_t& input
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

    template<typename sample_t, size_t BLOCK_SIZE>
    const inline std::pair<onepole_onezero_t<sample_t>, AudioBlock<sample_t,BLOCK_SIZE>> process(
        onepole_onezero_t<sample_t> state, 
        const AudioBlock<sample_t,BLOCK_SIZE>& input
    ){

        AudioBlock<sample_t, BLOCK_SIZE> output;
        for(size_t idx=0; idx<BLOCK_SIZE; idx++){

            state = process<sample_t>(state, input[idx]);
            output[idx] = state.y1;
        }

        return std::pair(state,output);
    }

}
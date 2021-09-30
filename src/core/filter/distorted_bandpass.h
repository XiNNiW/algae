#pragma once
#include <math.h>
#include "resonant_bandpass.h"
#include "../audio_block.h"
namespace algae::dsp::core::filter {
     template<typename sample_t>
    struct chaotic_resonator_t {
        reson_bp_t<sample_t> resonator;
        sample_t feedback_amt;
        sample_t chaos_gain;
    };

    template<typename sample_t, const sample_t (* fn)(const sample_t &), size_t BLOCKSIZE>
    inline const std::pair<chaotic_resonator_t<sample_t>, AudioBlock<sample_t,BLOCKSIZE>> process(
        chaotic_resonator_t<sample_t> r, 
        const AudioBlock<sample_t, BLOCKSIZE>& input
    )
    {
        const sample_t fb = r.feedback_amt;
        const sample_t g  = r.chaos_gain;
        AudioBlock<sample_t, BLOCKSIZE> block;
        for(size_t idx=0; idx<BLOCKSIZE; idx++){
            sample_t fb_signal = fn(fb*r.resonator.y1);
            
            fb_signal = input[idx] + g*fb_signal;
            fb_signal *= 0.5;
            r.resonator = process<sample_t>(r.resonator, fb_signal);
            block[idx] = r.resonator.y1;
        }

        return std::pair(r, block);
    }


}
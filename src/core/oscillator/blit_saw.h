#pragma once
#include <math.h>
#include "../audio_block.h"
namespace algae::dsp::core::oscillator {
    template<typename sample_t>
    struct stk_blit_saw_t{
        int m;
        sample_t phase_increment;
        sample_t phase;
        sample_t p;
        sample_t c2;
        sample_t a;
        sample_t state;
        sample_t lastFrame;
        
    };

    template<typename sample_t, typename frequency_t>
    const inline stk_blit_saw_t<sample_t> setFrequency(stk_blit_saw_t<sample_t> saw, const frequency_t& frequency, const frequency_t& sampleRate){

        saw.p = sampleRate/frequency;
        saw.c2 = 1/saw.p;
        saw.phase_increment = M_PI*saw.c2;

        int max_harmonics = floor(0.5*saw.p);
        saw.m = 2*max_harmonics+1;
        saw.a = saw.m / saw.p;

        return saw;
    }
    
    template<typename sample_t, typename frequency_t>
    const inline stk_blit_saw_t<sample_t> stk_blit_saw(const frequency_t& frequency, const frequency_t& sampleRate=44100){
        stk_blit_saw_t<sample_t> saw;

        saw = setFrequency(saw, frequency, sampleRate);
        saw.state = -0.5 * saw.a;
        return saw;
    }

    template<typename sample_t>
    const inline stk_blit_saw_t<sample_t> process(stk_blit_saw_t<sample_t> saw){
        sample_t tmp, denominator = sin( saw.phase );
        if ( fabs(denominator) <= std::numeric_limits<sample_t>::epsilon() )
            tmp = saw.a;
        else {
            tmp =  sin( saw.m * saw.phase );
            tmp /= saw.p * denominator;
        }

        tmp += saw.state - saw.c2;
        saw.state = tmp * 0.995;

        saw.phase += saw.phase_increment;
        if ( saw.phase >= M_PI ) saw.phase -= M_PI;
            
        saw.lastFrame = tmp;
        return saw;
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline std::pair<stk_blit_saw_t<sample_t>,AudioBlock<sample_t,BLOCKSIZE>> process(stk_blit_saw_t<sample_t> saw) {
        
        AudioBlock<sample_t,BLOCKSIZE> output;
        for(size_t idx=0;idx<BLOCKSIZE;idx++){
            saw = process<sample_t>(saw);
            output[idx] = saw.state;
        }
        
        return std::pair(saw,output);
    }

}
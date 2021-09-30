#pragma once
#include <math.h>
#include "../audio_block.h"
namespace algae::dsp::core::oscillator {
    template<typename sample_t>
    const sample_t noise(){
        return  2.0 * (( rand() /  RAND_MAX) - 0.5);
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const AudioBlock<sample_t,BLOCKSIZE> noise(){
        AudioBlock<sample_t,BLOCKSIZE> output;
        for(size_t idx=0; idx<BLOCKSIZE; idx++){
            output[idx] = noise<sample_t>();
        }
        return output;
    }
}
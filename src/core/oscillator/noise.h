#pragma once
#include <math.h>
#include "../audio_block.h"
namespace algae::dsp::core::oscillator {
    template<typename sample_t>
    const sample_t noise(){
        return  2.0 * (( static_cast<sample_t>(rand()) /  static_cast<sample_t>(RAND_MAX)) - 0.5);
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const AudioBlock<sample_t,BLOCKSIZE> noise(){
        AudioBlock<sample_t,BLOCKSIZE> output;
        for(size_t idx=0; idx<BLOCKSIZE; idx++){
            output[idx] = noise<sample_t>();
        }
        return output;
    }

    template<typename sample_t>
    void noise(const size_t& block_size, sample_t* out){
        for(size_t idx=0; idx<block_size; idx++)
            out[idx] = noise<sample_t>();
    }

    struct atari_noise_t{
        int phase=0;
    };

    template<typename sample_t>
    const atari_noise_t atari(
        atari_noise_t nz, const int& downsample_amount,
        const size_t& blocksize,
        sample_t* out
    ){
        for(size_t idx=0; idx<blocksize; idx++){
            if(nz.phase<downsample_amount){
                nz.phase++;
            } else {
                out[idx] = noise<sample_t>();
                nz.phase=0;
            }
        }

        return nz;
            
        
    }
}
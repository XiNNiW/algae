#pragma once
#include <math.h>
#include "../audio_block.h"
namespace algae::dsp::core::oscillator {
    template<typename sample_t>
    const sample_t noise(){
        //2.0 * rand() / (RAND_MAX + 1.0) - 1.0
        // auto random = static_cast<sample_t>(rand());
        // const auto random_max_value = static_cast<sample_t>(RAND_MAX));

        return  static_cast<sample_t>(2.0 * rand() / (RAND_MAX + 1.0) - 1.0);
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
    const atari_noise_t process(
        atari_noise_t nz, const sample_t* downsample_amount,
        const size_t& blocksize,
        sample_t* out
    ){
        for(size_t idx=0; idx<blocksize; idx++){
            if(nz.phase<int(downsample_amount[idx])){
                nz.phase++;
                out[idx]=0;
            } else {
                out[idx] = noise<sample_t>();
                nz.phase=0;
            }
        }

        return nz;
            
    }
}
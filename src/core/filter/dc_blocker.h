#pragma once
#include <math.h>
#include "../audio_block.h"

namespace algae::dsp::core::filter{

    // Yates and Lyons, DC Blocker Algorithms, http://www.digitalsignallabs.com/downloads/dcblock.pdf
    template<typename sample_t>
    struct dc_block_t{
        sample_t y1;
        sample_t x1;
        // sample_t frac;
    };

    template<typename sample_t>
    const inline dc_block_t<sample_t> process(dc_block_t<sample_t> i, const sample_t input){
        const sample_t xn = input;
        // // this was causing the kind of DC errors that the authors were associating with fixed point math but i was using doubles?!!!
        // const sample_t p = 0.995;
        // // const sample_t f = i.frac;
        // const sample_t pyn1 = p*i.y1;
        // const sample_t frac = pyn1-floor(pyn1); // noise shaping as suggested by authors for resolving fixed point quantization errors??
        // const sample_t yn = xn - i.x1 + pyn1 - frac; 
        // return dc_block_t<sample_t>{yn,xn};
        i.y1 = input - i.x1 + 0.995*i.y1;
        i.x1 = input;
        return i;
    }

    template<typename sample_t, size_t BLOCK_SIZE>
    const inline std::pair<dc_block_t<sample_t>, AudioBlock<sample_t, BLOCK_SIZE>> process(
        dc_block_t<sample_t> state, 
        const AudioBlock<sample_t,BLOCK_SIZE>& input
    ){
        
        AudioBlock<sample_t, BLOCK_SIZE> output;
        for(size_t idx=0; idx<BLOCK_SIZE; idx++){
            state = process<sample_t>(state, input[idx]);
            output[idx] = state.y1;
        }

        return std::pair<dc_block_t<sample_t>, AudioBlock<sample_t, BLOCK_SIZE>>(state,output);
    }

}
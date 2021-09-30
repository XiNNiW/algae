#include <math.h>
#pragma once
namespace algae::dsp::core::filter{

    // Yates and Lyons, DC Blocker Algorithms, http://www.digitalsignallabs.com/downloads/dcblock.pdf
    template<typename sample_t>
    struct dc_block_t{
        sample_t y1;
        sample_t x1;
        // sample_t frac;
    };

    template<typename sample_t>
    dc_block_t<sample_t> process_dc_blocker(const dc_block_t<sample_t> i, const sample_t input){
        const sample_t xn = input;
        // this was causing the kind of DC errors that the authors were associating with fixed point math but i was using doubles?!!!
        const sample_t p = 0.995;
        // const sample_t f = i.frac;
        const sample_t pyn1 = p*i.y1;
        const sample_t frac = pyn1-floor(pyn1); // noise shaping as suggested by authors for resolving fixed point quantization errors??
        const sample_t yn = xn - i.x1 + pyn1 - frac; 
        return dc_block_t<sample_t>{yn,xn};
    }
}
#pragma once
#include "../math_tools.h"
#include "../constants.h"
#include "../audio_block.h"

namespace algae::dsp::core::control {


    template<typename sample_t>
    const sample_t blackman_harris(const sample_t& phase){
        //     w[n] = a_0 - a_1cos(\frac{2\pi\n}{N}) + a_2cos(\frac{4\pi\n}{N}) - a_3cos(\frac{6\pi\n}{N})
        // a_0 = 0.35875; a_1 = 0.48829; a_2 = 0.14128; a_3 = 0.01168
        using algae::dsp::core::math::cos;
        constexpr sample_t FOUR_PI = 2*TWO_PI;
        constexpr sample_t SIX_PI = 3*TWO_PI;
        sample_t a_0 = 0.35875; 
        sample_t a_1 = 0.48829; 
        sample_t a_2 = 0.14128;
        sample_t a_3 = 0.01168;
        
        return a_0 - a_1*cos(TWO_PI*phase) +  a_2*cos(FOUR_PI*phase) - a_3*cos(SIX_PI*phase);
    }

    template<typename sample_t>
    const sample_t hann(const sample_t& phase){
        // w[n] = \frac{1}{2} [1 - cos(\frac{2\pi n}{N})] = sin^2 (\frac{\pi n}{N})
        using algae::dsp::core::math::cos;
        
        return 0.5*(1-cos(TWO_PI*phase));
    }

    using algae::dsp::core::AudioBlock;
    template<typename sample_t, size_t N, const sample_t (* fn)(const sample_t &)>
    AudioBlock<sample_t,N> apply_window(AudioBlock<sample_t,N> block){
        for(size_t idx=0; idx<N; idx++){
            block[idx] = block[idx]*fn(sample_t(idx)/sample_t(N));
        }
        return block;
    }
        
    template<typename sample_t, const sample_t (* fn)(const sample_t &)>
    void apply_window(const sample_t* block, const size_t& N, sample_t* out){
        for(size_t idx=0; idx<N; idx++){
            out[idx] = block[idx]*fn(sample_t(idx)/sample_t(N));
        }
        
    }


}
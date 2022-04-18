#pragma once
#include <math.h>
#include <utility>
#include "audio_block.h"
#include "math_tools.h"
#include "oscillator.h"
#include "constants.h"

namespace algae::dsp::core::analysis {
    template<typename sample_t, size_t POLES, size_t SAMPLES>
    std::array<sample_t, SAMPLES*POLES> lpc_matrix_create(const std::array<sample_t, SAMPLES+POLES-1> &x){
        std::array<sample_t, SAMPLES*POLES> A;
        for(size_t i = 0; i<SAMPLES; i++){
            for(size_t p = 0; p<POLES; p++){
                A[i*POLES + p] = x[i+p];
            }
        }
        return A;
    }

    template<typename sample_t, size_t TO>
    std::array<sample_t, TO> truncate(sample_t* x){
        std::array<sample_t, TO> to;
        for(size_t i = 0; i<TO; i++){
            to[i] = x[i];
        }
        return to;
    }

    using  algae::dsp::core::math::moore_penrose_pseudoinverse;
    using  algae::dsp::core::math::matrix_mult;

    template<typename sample_t, size_t POLES, size_t SAMPLES>
    const inline std::array<sample_t, POLES> lpc_predict(const std::array<sample_t, SAMPLES+POLES-1> &x){
        std::array<sample_t, POLES*SAMPLES> At = moore_penrose_pseudoinverse<sample_t, POLES, SAMPLES>(
            lpc_matrix_create<sample_t, POLES, SAMPLES>(x)
        );
        std::array<sample_t, SAMPLES> b = truncate<sample_t,SAMPLES>(x.data());
        return matrix_mult<sample_t, POLES, SAMPLES, 1>(At, b);
    }





}
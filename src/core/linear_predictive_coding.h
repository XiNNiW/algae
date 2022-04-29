#pragma once
#include <math.h>
#include <utility>
#include "audio_block.h"
#include "math_tools.h"
#include "oscillator.h"
#include "constants.h"

using  algae::dsp::core::math::moore_penrose_pseudoinverse;
using  algae::dsp::core::math::matrix_mult;
using  algae::dsp::core::math::transpose;
using  algae::dsp::core::math::toeplitz_matrix_create;
using  algae::dsp::core::math::autocorrelation;

namespace algae::dsp::core::analysis {
    using  algae::dsp::core::math::moore_penrose_pseudoinverse;
    using  algae::dsp::core::math::matrix_mult;
    using  algae::dsp::core::math::transpose;
    using  algae::dsp::core::math::toeplitz_matrix_create;
    using  algae::dsp::core::math::autocorrelation;
    using  algae::dsp::core::math::inverse;


    template<typename sample_t>
    const inline sample_t lpc_biased_autocorrelation(const sample_t* x, const size_t &x_length, const int &l){
        sample_t r = 0;
        for(int n=0; n<static_cast<int>(x_length)-l; n++){
            r+=x[n]*x[n+l];
        }

        return r;

    }

    template<typename sample_t>
    sample_t lpc_levinson(
        const sample_t* x, const size_t &x_length, const size_t &order,
        /*=>*/  sample_t* a, sample_t* k
    ){
        //https://authors.library.caltech.edu/25063/1/S00086ED1V01Y200712SPR003.pdf

        sample_t a_flip[order];
        sample_t R[order+1];
        for(size_t n=0; n<=order; n++){ 
            R[n] = lpc_biased_autocorrelation<sample_t>(x, x_length, n);
        }

        a[0] = 1.0000;
        sample_t alpha_m = R[1];
        sample_t e_m = R[0];
        k[0] = -alpha_m/e_m;

        for(size_t n=1; n<order; n++){
            
            alpha_m = R[n];
            a[n]=0;

            for(size_t m=1; m<n; m++)
            {
                alpha_m += a[m]*R[n-m];
            }
            for(size_t m=0; m<n; m++)
            {
                a_flip[m] = a[n-m-1];
            }

            k[n] = -alpha_m/e_m;
            for(size_t m=1; m<=n; m++)
            {
                a[m] += k[n]*a_flip[m-1];
            }

            e_m = (1 - k[n]*k[n])*e_m;
        }
        

        return e_m;
        
    }

   


}
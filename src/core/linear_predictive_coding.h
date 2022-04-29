#pragma once
#include <math.h>
#include <utility>
#include "audio_block.h"
#include "oscillator.h"
#include "constants.h"

namespace algae::dsp::core::analysis {

    template<typename sample_t, size_t ORDER>
    struct lpc_t{
        sample_t error;
        std::array<sample_t, ORDER> filter_coefficients;
        std::array<sample_t, ORDER> reflection_coefficients;
    };

    template<typename sample_t, size_t BLOCK_SIZE>
    const inline sample_t biased_autocorrelation(const std::array<sample_t, BLOCK_SIZE> &x, const int &l){
        sample_t r = 0;
        for(int n=0; n<static_cast<int>(BLOCK_SIZE)-l; n++){
            r+=x[n]*x[n+l];
        }

        return r;

    }

    template<typename sample_t, size_t ORDER, size_t BLOCK_SIZE>
    lpc_t<sample_t, ORDER> lpc_analyze(
        const std::array<sample_t, BLOCK_SIZE> &x
    ){
        //https://authors.library.caltech.edu/25063/1/S00086ED1V01Y200712SPR003.pdf
        std::array<sample_t, ORDER> a;
        std::array<sample_t, ORDER> k;
        std::array<sample_t, ORDER> a_flip;
        std::array<sample_t, ORDER+1> R;
        for(size_t n=0; n<=ORDER; n++){ 
            R[n] = biased_autocorrelation<sample_t,BLOCK_SIZE>(x, n);
        }

        a[0] = 1;
        sample_t alpha_m = R[1];
        sample_t e_m = R[0];
        k[0] = -alpha_m/e_m;

        for(size_t n=1; n<ORDER; n++){
            
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

        
        lpc_t<sample_t, ORDER> result;
        result.filter_coefficients = a;
        result.reflection_coefficients = k;
        result.error = e_m;

        return result;
        
    }


    // template<typename sample_t>
    // const inline sample_t lpc_biased_autocorrelation(const sample_t* x, const size_t &x_length, const int &l){
    //     sample_t r = 0;
    //     for(int n=0; n<static_cast<int>(x_length)-l; n++){
    //         r+=x[n]*x[n+l];
    //     }

    //     return r;

    // }

    // template<typename sample_t>
    // sample_t lpc_levinson(
    //     const sample_t* x, const size_t &x_length, const size_t &order,
    //     /*=>*/  sample_t* a, sample_t* k
    // ){
    //     //https://authors.library.caltech.edu/25063/1/S00086ED1V01Y200712SPR003.pdf

    //     sample_t a_flip[order];
    //     sample_t R[order+1];
    //     for(size_t n=0; n<=order; n++){ 
    //         R[n] = lpc_biased_autocorrelation<sample_t>(x, x_length, n);
    //     }

    //     a[0] = 1.0000;
    //     sample_t alpha_m = R[1];
    //     sample_t e_m = R[0];
    //     k[0] = -alpha_m/e_m;

    //     for(size_t n=1; n<order; n++){
            
    //         alpha_m = R[n];
    //         a[n]=0;

    //         for(size_t m=1; m<n; m++)
    //         {
    //             alpha_m += a[m]*R[n-m];
    //         }
    //         for(size_t m=0; m<n; m++)
    //         {
    //             a_flip[m] = a[n-m-1];
    //         }

    //         k[n] = -alpha_m/e_m;
    //         for(size_t m=1; m<=n; m++)
    //         {
    //             a[m] += k[n]*a_flip[m-1];
    //         }

    //         e_m = (1 - k[n]*k[n])*e_m;
    //     }
        

    //     return e_m;
        
    // }

   


}
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

    template<typename sample_t, size_t N>
    const inline sample_t biased_autocorrelation(const std::array<sample_t, N>& x, const size_t &l){

        if(l>=N) return 0;

        sample_t a = 0;
        for(size_t n=0; n<N-l; n++){
            a += x[n]*x[n+l];
        }
        return a;

    }

    template<typename sample_t, size_t N>
    const inline std::array<sample_t, N> reverse(const std::array<sample_t, N>& x){

        std::array<sample_t, N> xr;
        for(size_t i=0; i<N; i++)\
            xr[N-1-i] = x[i];
        return xr;
    }

    
    template<typename sample_t, size_t SAMPLES, size_t ORDER>
    std::array<sample_t, SAMPLES*ORDER> autocorrelation_matrix_create(const std::array<sample_t, SAMPLES> &x){
        std::array<sample_t, SAMPLES*ORDER> A;
        for(int i = 0; i<static_cast<int>(SAMPLES); i++){
            for(int p = 0; p<static_cast<int>(ORDER); p++){
                A[i*ORDER + p] = biased_autocorrelation<sample_t, SAMPLES>(x, abs(i-p));
            }
        }
        return A;
    }

    template<typename sample_t, size_t SAMPLES, size_t ORDER>
    inline std::pair<std::array<sample_t, ORDER>, sample_t >lpc_durbin(const std::array<sample_t, SAMPLES*ORDER> &r){
        std::array<sample_t, ORDER+1> a, at;
        std::array<sample_t, ORDER> k;
        auto e = r[0];
        for (int i=0; i<=static_cast<int>(ORDER); i++) a[i] = at[i] = 0.0;

        for(int i=1; i<=static_cast<int>(ORDER); i++) {
            k[i] = -r[i];

            for(int j=1; j<i; j++) {
            at[j] = a[j];
            k[i] -= a[j] * r[i-j];
            }

            if (fabs(e) < 1.0e-20f) {
                e = 0.0;  break;
            }

            k[i] /= e;

            a[i] = k[i];
            for(int j=1; j<i; j++) a[j] = at[j] + k[i] * at[i-j];

            e *= 1.0 - k[i] * k[i];
        }

        if(e < 1.0e-20f) e = 0.0f;

        return std::pair(k,sqrt(e));
    }

    template<typename sample_t>
    const inline sample_t lpc_biased_autocorrelation(const sample_t* x, const size_t &x_length, const int &l){
        sample_t r = 0;
        for(int n=0; n<static_cast<int>(x_length)-l; n++){
            r+=x[n]*x[n+l];
        }

        return r;

    }

    template<typename sample_t>
    void lpc_levinson(
        const sample_t* x, const size_t &x_length, const size_t &order,
        /*=>*/  sample_t* a, sample_t* k, sample_t* e
    ){
        //https://authors.library.caltech.edu/25063/1/S00086ED1V01Y200712SPR003.pdf

        sample_t a_flip[order];
        sample_t R[order+1];
        for(size_t n=0; n<=order; n++){ 
            R[n] = lpc_biased_autocorrelation<sample_t>(x, x_length, n);
        }

        a[0] = 1.0;
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
        
        
    }

    template<typename sample_t, size_t SAMPLES, size_t ORDER>
    const inline std::pair<std::array<sample_t, ORDER>, sample_t> lpc_analyze(const std::array<sample_t, SAMPLES> &x){
        auto R = autocorrelation_matrix_create<sample_t, SAMPLES, ORDER>(x);
        R[0] *= 1.001;  /* stability fix */

        sample_t min = 0.00001;
        if(R[0] < min) { 
            std::array<sample_t, ORDER> a;
            sample_t G = 0.0;
            for(size_t i=0; i<ORDER; i++) a[i] = 0.0; 
            return std::pair(a, G); 
        }

        return lpc_durbin<sample_t, SAMPLES, ORDER>(R);

    }

    template<typename sample_t, size_t ORDER, size_t SAMPLES>
    std::array<sample_t, SAMPLES*ORDER> lpc_matrix_create(const std::array<sample_t, SAMPLES> &x){
        std::array<sample_t, SAMPLES*ORDER> A;
        for(int i = 0; i<static_cast<int>(SAMPLES); i++){
            for(int p = 0; p<static_cast<int>(ORDER); p++){
                size_t signal_idx = i+p;
                // A[i*ORDER + p] = x[signal_idx%SAMPLES];
               A[i*ORDER + p] = signal_idx<SAMPLES?x[signal_idx]:0;
            // toeplitz[i*N + j] = in[abs(static_cast<int>(i)-static_cast<int>(j))];

            //    A[i*ORDER + p] = x[abs(static_cast<int>(i)-static_cast<int>(p))];
            }
        }
        return A;
    }

    template<typename sample_t, size_t TO>
    std::array<sample_t, TO> truncate(const sample_t* x){
        std::array<sample_t, TO> to;
        for(size_t i = 0; i<TO; i++){
            to[i] = x[i];
        }
        return to;
    }

    template<typename sample_t, size_t LEN>
    const inline std::array<sample_t, LEN+1> append(const std::array<sample_t, LEN> &A, sample_t value){
        std::array<sample_t, LEN+1> appended;
        for(size_t idx=0; idx<LEN; idx++)
            appended[idx] = A[idx];

        appended[LEN+1] = value;
        return appended;
    }


    

    template<typename sample_t, size_t ORDER, size_t SAMPLES>
    const inline std::array<sample_t, ORDER> lpc_predict(const std::array<sample_t, SAMPLES> &x){
      
        std::array<sample_t, ORDER*SAMPLES> At = moore_penrose_pseudoinverse<sample_t, ORDER, SAMPLES>(
            lpc_matrix_create<sample_t, ORDER, SAMPLES>(x)
        );
        return matrix_mult<sample_t, ORDER, SAMPLES, 1>(At, x);
        // auto result = matrix_mult<sample_t, ORDER, SAMPLES, 1>(At, x);
        // for(size_t idx=0; idx<ORDER; idx++)
        //     result[idx] = result[idx]/result[0];
        // return result;

    }


}
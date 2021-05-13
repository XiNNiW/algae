#pragma once
#include <math.h>
#include "audio_block.h"

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::math{

    template<typename sample_t, size_t BLOCKSIZE, const sample_t (* fn)(const sample_t &)>
    const inline AudioBlock<sample_t, BLOCKSIZE> unary_block_op(const AudioBlock<sample_t,BLOCKSIZE>& x)
    {
        AudioBlock<sample_t,BLOCKSIZE> output;
        for(size_t idx=0;idx<BLOCKSIZE;idx++){
            output[idx] = fn(x[idx]);
        }
        return output;
    }

    template<typename sample_t, size_t BLOCKSIZE, const sample_t (* fn)(const sample_t &)>
    const inline AudioBlock<sample_t, BLOCKSIZE> binary_block_op(const AudioBlock<sample_t,BLOCKSIZE>& x,const AudioBlock<sample_t,BLOCKSIZE>& y)
    {
        AudioBlock<sample_t,BLOCKSIZE> output;
        for(size_t idx=0;idx<BLOCKSIZE;idx++){
            output[idx] = fn(x[idx], y[idx]);
        }
        return output;
    }

    template<typename sample_t, size_t BLOCKSIZE, const sample_t (* fn)(const sample_t &)>
    const inline AudioBlock<sample_t, BLOCKSIZE> binary_block_op(const AudioBlock<sample_t,BLOCKSIZE>& x, const sample_t& y)
    {
        AudioBlock<sample_t,BLOCKSIZE> output;
        for(size_t idx=0;idx<BLOCKSIZE;idx++){
            output[idx] = fn(x[idx], y);
        }
        return output;
    }

    template<typename sample_t, size_t BLOCKSIZE, const sample_t (* fn)(const sample_t &,const sample_t &)>
    const inline AudioBlock<sample_t, BLOCKSIZE> binary_block_op(const sample_t& x, const AudioBlock<sample_t,BLOCKSIZE>& y)
    {
        AudioBlock<sample_t,BLOCKSIZE> output;
        for(size_t idx=0;idx<BLOCKSIZE;idx++){
            output[idx] = fn(x[idx], y);
        }
        return output;
    }

    template<typename sample_t>
    const inline sample_t tanh_approx_pade(const sample_t& x)
    {
        if( x < -3 )
            return -1;
        else if( x > 3 )
            return 1;
        else
            return x * ( 27 + x * x ) / ( 27 + 9 * x * x );
    }


    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> tanh_approx_pade(const AudioBlock<sample_t,BLOCKSIZE>& x) {
        return unary_block_op<sample_t,BLOCKSIZE,tanh_approx_pade<sample_t>>(x);
    };

    template<typename sample_t>
    const inline sample_t tanh_approx_pade_noclip(const sample_t& x)
    {
        return x * ( 27 + x * x ) / ( 27 + 9 * x * x );
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> tanh_approx_pade_noclip(const AudioBlock<sample_t,BLOCKSIZE>& x) {
        return unary_block_op<sample_t,BLOCKSIZE,tanh_approx_pade_noclip<sample_t>>(x);
    };

    template<typename sample_t, typename frequency_t>
    const sample_t sin(const sample_t& phase){
        return math::sin(phase);
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> sin(const AudioBlock<sample_t,BLOCKSIZE>& x) {
        return unary_block_op<sample_t,BLOCKSIZE,sin<sample_t>>(x);
    };

    template<typename sample_t, typename frequency_t>
    const sample_t cos(const sample_t& phase){
        return math::cos(phase);
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> cos(const AudioBlock<sample_t,BLOCKSIZE>& x) {
        return unary_block_op<sample_t,BLOCKSIZE,cos<sample_t>>(x);
    };

    template<typename sample_t, typename frequency_t>
    const sample_t tan(const sample_t& phase){
        return math::tan(phase);
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> tan(const AudioBlock<sample_t,BLOCKSIZE>& x) {
        return unary_block_op<sample_t,BLOCKSIZE,tan<sample_t>>(x);
    };

    template<typename sample_t, typename frequency_t>
    const sample_t tanh(const sample_t& phase){
        return math::tanh(phase);
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> tanh(const AudioBlock<sample_t,BLOCKSIZE>& x) {
        return unary_block_op<sample_t,BLOCKSIZE,tanh<sample_t>>(x);
    };

    template<typename sample_t, typename frequency_t>
    const sample_t sqrt(const sample_t& phase){
        return math::sqrt(phase);
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> sqrt(const AudioBlock<sample_t,BLOCKSIZE>& x) {
        return unary_block_op<sample_t,BLOCKSIZE,sqrt<sample_t>>(x);
    };

    template<typename sample_t>
    const sample_t abs(const sample_t& x){
        return x<0?-x:x;
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> abs(const AudioBlock<sample_t,BLOCKSIZE>& x) {
        return unary_block_op<sample_t,BLOCKSIZE,abs<sample_t>>(x);
    };



    template<typename sample_t, typename frequency_t>
    const sample_t wrap(const sample_t& x){
        return x-floor(x);
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> wrap(const AudioBlock<sample_t,BLOCKSIZE>& x) {
        return unary_block_op<sample_t,BLOCKSIZE,wrap<sample_t>>(x);
    };

    template<typename sample_t>
    const sample_t clamp(const sample_t& x, const sample_t& lo, const sample_t& hi){
        return fmin(fmax(x, lo), hi);
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> clamp(const AudioBlock<sample_t,BLOCKSIZE>& x, const AudioBlock<sample_t,BLOCKSIZE>& y) {
        return binary_block_op<sample_t,BLOCKSIZE,clamp<sample_t>>(x,y);
    };

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> clamp(const AudioBlock<sample_t,BLOCKSIZE>& x, const sample_t& y) {
        return binary_block_op<sample_t,BLOCKSIZE,clamp<sample_t>>(x,y);
    };

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> clamp( const sample_t& x, const AudioBlock<sample_t,BLOCKSIZE>& y) {
        return binary_block_op<sample_t,BLOCKSIZE,clamp<sample_t>>(x,y);
    };


    template<typename sample_t, typename frequency_t>
    const sample_t wrapto(const sample_t& x, const sample_t& y){
        return wrap(x)*y;
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> wrapto(const AudioBlock<sample_t,BLOCKSIZE>& x, const AudioBlock<sample_t,BLOCKSIZE>& y) {
        return binary_block_op<sample_t,BLOCKSIZE,wrapto<sample_t>>(x,y);
    };

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> wrapto(const AudioBlock<sample_t,BLOCKSIZE>& x, const sample_t& y) {
        return binary_block_op<sample_t,BLOCKSIZE,wrapto<sample_t>>(x,y);
    };

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> wrapto( const sample_t& x, const AudioBlock<sample_t,BLOCKSIZE>& y) {
        return binary_block_op<sample_t,BLOCKSIZE,wrapto<sample_t>>(x,y);
    };



    template<typename sample_t, typename frequency_t>
    const sample_t wrapat(const sample_t& x, const sample_t& y){
        return wrap(x)/y;
    }


    template<typename sample_t, typename frequency_t>
    const sample_t fmod(const sample_t& x, const sample_t& y){
        return wrap(x/y)*y;
    }



    template<typename sample_t>
    const sample_t multiply(const sample_t& lhs,const sample_t& rhs){
        return lhs*rhs;
    }

    template<typename sample_t>
    const sample_t divide(const sample_t& lhs,const sample_t& rhs){
        return lhs/rhs;
    }

    template<typename sample_t>
    const sample_t add(const sample_t& lhs,const sample_t& rhs){
        return lhs+rhs;
    }

    template<typename sample_t>
    const sample_t subtract(const sample_t& lhs,const sample_t& rhs){
        return lhs-rhs;
    }

    template<typename sample_t>
    const sample_t power(const sample_t& lhs,const sample_t& rhs){
        return pow(lhs,rhs);
    }
}

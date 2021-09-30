#pragma once
#include "factorial.h"
#include <array>
namespace algae::dsp::core::oscillator {
    template<typename sample_t>
    constexpr const sample_t _sin(const sample_t phase){
        sample_t x = phase;
        sample_t x2 = x*x;
        sample_t x3 = x2*x;
        sample_t x5 = x2*x3;
        sample_t x7 = x2*x5;
        sample_t x9 = x2*x7;
        sample_t x11 = x2*x9;
        sample_t x13 = x2*x11;
        sample_t x15 = x2*x13;
        sample_t x17 = x2*x15;
        sample_t x19 = x2*x17;

        return x
            -(x3/factorial_t<sample_t,3>::result)
            +(x5/factorial_t<sample_t,5>::result)
            -(x7/factorial_t<sample_t,7>::result)
            +(x9/factorial_t<sample_t,9>::result)
            -(x11/factorial_t<sample_t,11>::result)
            +(x13/factorial_t<sample_t,13>::result)
            -(x15/factorial_t<sample_t,15>::result)
            +(x17/factorial_t<sample_t,17>::result)
            -(x19/factorial_t<sample_t,19>::result)
            ;
    }



    template<typename sample_t, int TABLE_SIZE, std::size_t... INDEX>
    constexpr const std::array<sample_t, TABLE_SIZE> _makeSineTable(const std::index_sequence<INDEX...> index_seq){
       
        const sample_t phase_increment = 2.0*M_PI/sample_t(TABLE_SIZE);
        return {{_sin<sample_t>(sample_t(INDEX)*phase_increment)...}};

    }

    template<typename sample_t, int TABLE_SIZE, typename Indices = std::make_index_sequence<TABLE_SIZE>>
    constexpr const std::array<sample_t, TABLE_SIZE> makeSineTable(){
        return _makeSineTable<sample_t, TABLE_SIZE>(Indices{});
    }


    template<typename sample_t, size_t TABLE_SIZE>
    struct sine_t{
        static constexpr std::array<sample_t, TABLE_SIZE> TABLE = makeSineTable<sample_t,TABLE_SIZE>();
        inline static const sample_t lookup(const sample_t& phase){
            return table_lookup_lin_interp<sample_t,TABLE_SIZE>(TABLE, phase);
        }

    };

    template<typename sample_t>
    struct sine_t<sample_t,0>{
        inline static const sample_t lookup(const sample_t& phase){
            return sin(TWO_PI*phase);
        }

    };

    template<typename sample_t, size_t TABLE_SIZE, size_t BLOCK_SIZE>
    struct sineOsc{
        inline static const std::pair<sample_t, AudioBlock<sample_t,BLOCK_SIZE>>process(sample_t phase, const sample_t& phi){
            AudioBlock<sample_t, BLOCK_SIZE> output;
            for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                output[idx] = sine_t<sample_t,TABLE_SIZE>::lookup(phase);
                phase = update_phase<sample_t,sample_t>(phase, phi, 1.0);
            }
            return std::pair(phase, output);
        }
    };


}
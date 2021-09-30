#pragma once
#include "factorial.h"
#include <array>
namespace algae::dsp::core::oscillator {
    template<typename sample_t>
    constexpr const sample_t _cos(const sample_t phase){
        sample_t x = phase;
        sample_t x2 = x*x;
        sample_t x4 = x2*x2;
        sample_t x6 = x2*x4;
        sample_t x8 = x2*x6;
        sample_t x10 = x2*x8;
        sample_t x12 = x2*x10;
        sample_t x14 = x2*x12;
        sample_t x16 = x2*x14;
        sample_t x18 = x2*x16;

        return 1
            -(x2/factorial_t<sample_t,2>::result)
            +(x4/factorial_t<sample_t,4>::result)
            -(x6/factorial_t<sample_t,6>::result)
            +(x8/factorial_t<sample_t,8>::result)
            -(x10/factorial_t<sample_t,10>::result)
            +(x12/factorial_t<sample_t,12>::result)
            -(x14/factorial_t<sample_t,14>::result)
            +(x16/factorial_t<sample_t,16>::result)
            -(x18/factorial_t<sample_t,18>::result)
            ;

    }

    template<typename sample_t, int TABLE_SIZE, std::size_t... INDEX>
    constexpr const std::array<sample_t, TABLE_SIZE> _makeCosTable(const std::index_sequence<INDEX...> index_seq){
       
        const sample_t phase_increment = 2.0*M_PI/sample_t(TABLE_SIZE);
        return {{_cos<sample_t>(sample_t(INDEX)*phase_increment)...}};

    }

    template<typename sample_t, int TABLE_SIZE, typename Indices = std::make_index_sequence<TABLE_SIZE>>
    constexpr const std::array<sample_t, TABLE_SIZE> makeCosTable(){
        return _makeCosTable<sample_t,TABLE_SIZE>(Indices{});
    }

    
    template<typename sample_t, int TABLE_SIZE>
    struct cos_t{
        static constexpr std::array<sample_t, TABLE_SIZE> TABLE = makeCosTable<sample_t,TABLE_SIZE>();
        inline static const sample_t lookup(const sample_t& phase){
            return table_lookup_lin_interp<sample_t,TABLE_SIZE>(TABLE, phase);
        }
        
    };

    template<typename sample_t>
    struct cos_t<sample_t, 0>{
        inline static const sample_t lookup(const sample_t& phase){
            return cos(TWO_PI*phase);
        }
    };
}
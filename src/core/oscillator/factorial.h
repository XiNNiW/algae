#pragma once
#include <math.h>
#include "../constants.h"
namespace algae::dsp::core::oscillator {
    template<typename sample_t, unsigned int x>
    struct factorial_t{
        static constexpr sample_t result=x*factorial_t<sample_t, x-1>::result;
    };

    template<typename sample_t>
    struct factorial_t<sample_t,1>{
        static constexpr sample_t result=1;
    };
}
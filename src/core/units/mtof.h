#pragma once
#include <math.h>
namespace algae::dsp::core::units {
    template<typename frequency_t>
    inline constexpr frequency_t mtof(const frequency_t& n){
        const frequency_t REF_FREQ = 440.0;
        const int REF_NOTE = 69;
        return  REF_FREQ * pow (2, (n - REF_NOTE) / 12);
    };

}
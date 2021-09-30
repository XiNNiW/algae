#pragma once
#include <math.h>
#include "../constants.h"
namespace algae::dsp::core::oscillator {
    template<typename sample_t, typename frequency_t>
    const inline sample_t update_phase(sample_t phase, const frequency_t& increment, const frequency_t& period=1){
        phase += period*increment;
        if ( phase >= period ) phase -= period;
        return phase;
    }
}
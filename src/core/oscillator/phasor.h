#pragma once
#include <math.h>
#include "../constants.h"

namespace algae::dsp::core::oscillator {
    template<typename frequency_t>
    const inline frequency_t compute_phase_increment(const frequency_t& frequency, const frequency_t& sample_rate){
        return frequency/sample_rate;
    }

    template<typename sample_t>
    const inline sample_t update_phase(sample_t phase, const sample_t& increment, const sample_t& period=1){
        phase += period*increment;
        if ( phase >= period ) phase -= period;
        return phase;
    }
}
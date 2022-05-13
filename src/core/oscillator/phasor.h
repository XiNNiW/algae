#pragma once
#include <math.h>
#include "../constants.h"
#include "../simd.h"

namespace algae::dsp::core::oscillator {
    using algae::dsp::core::simd::simd_traits;
    using algae::dsp::core::simd::select;

    template<typename frequency_t>
    const inline frequency_t compute_phase_increment(const frequency_t& frequency, const frequency_t& sample_rate){
        return frequency/sample_rate;
    }

    template<typename sample_t, typename frequency_t>
    const inline sample_t update_phase(sample_t phase, const frequency_t& increment, const frequency_t& period=1){
        phase += period*increment;
        if ( phase >= period ) phase -= period;
        return phase;
    }

    template<typename sample_t>
    const inline typename simd_traits<sample_t>::type update_phase(typename simd_traits<sample_t>::type phase, const typename simd_traits<sample_t>::type& increment, const typename simd_traits<sample_t>::type& period=1){
        phase += period*increment;
        auto reset_condition = phase >= period;
        auto wrappedPhase = phase - period;
        return select(reset_condition, wrappedPhase, phase);
        
    }
}
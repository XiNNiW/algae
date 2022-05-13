#pragma once
#include <math.h>
#include "../constants.h"
#include "../simd.h"

namespace algae::dsp::core::oscillator {
    using algae::dsp::core::simd::simd_traits;
    using algae::dsp::core::simd::simd_vector_traits;
    using algae::dsp::core::simd::select;

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


    // template<typename sample_t, typename vec_t = typename simd_traits<sample_t>::type>
    // const inline vec_t update_phase_vec(vec_t phase, const vec_t& increment, const vec_t& period=1){
    //     typedef typename simd_vector_traits<vec_t>::vector_bool vec_bool_t;
    //     phase += period*increment;
    //     vec_bool_t reset_condition = phase >= period;
    //     vec_t wrappedPhase = phase - period;
    //     return select(reset_condition, wrappedPhase, phase);
        
    // }
}
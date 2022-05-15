#pragma once
#include <iostream>
#include <math.h>
#include "../constants.h"
#include "../simd.h"

namespace algae::dsp::core::oscillator {
    using algae::dsp::core::simd::simd_traits;
    using algae::dsp::core::simd::simd_vector_traits;
    using algae::dsp::core::simd::select;

    using algae::dsp::core::simd::simd_traits;
    using algae::dsp::core::simd::select;
    using algae::dsp::core::simd::load_a;
    using algae::dsp::core::simd::load_u;

    template <class sample_t, class vec_t>
    struct phasor_invoker
    {
        static const inline vec_t update_phase(const vec_t& previous_phase, const vec_t& increment, const vec_t& period=1){
            
            constexpr size_t vec_size = simd_traits<sample_t>::size;
            sample_t phase = previous_phase[vec_size-1];
            vec_t period_times_increment = period*increment;
            alignas(vec_size) sample_t next_phase[vec_size];
            for(size_t idx=0; idx<vec_size; idx++){
                phase += period_times_increment[idx];
                next_phase[idx] = phase;
            }
            vec_t next_phase_vec = load_a(next_phase);
            auto reset_condition = next_phase_vec >= period;
            next_phase_vec = select(reset_condition, next_phase_vec-period, next_phase_vec);


            return next_phase_vec;
        }
    };

    template <class sample_t>
    struct phasor_invoker<sample_t, sample_t>
    {
        static const inline sample_t update_phase(sample_t phase, const sample_t& increment, const sample_t& period=1){
            phase += period*increment;
            if ( phase >= period ) phase -= period;
            return phase;
        }
    };

    template<typename frequency_t>
    const inline frequency_t compute_phase_increment(const frequency_t& frequency, const frequency_t& sample_rate){
        return frequency/sample_rate;
    }

    template<typename sample_t>
    const inline sample_t update_phase(const sample_t& phase, const sample_t& increment, const sample_t& period=1) {
        // phase += period*increment;
        // if ( phase >= period ) phase -= period;
        // return phase;
        return phasor_invoker<typename simd_vector_traits<sample_t>::value_type, sample_t>::update_phase(phase, increment, period);
    }


    // template<typename sample_t, typename vec_t = typename simd_traits<sample_t>::type>
    // const inline vec_t update_phase_vec(vec_t phase, const vec_t& increment, const vec_t& period=1){
    //     // typedef typename simd_vector_traits<vec_t>::vector_bool vec_bool_t;
    //     // phase += period*increment;
    //     // vec_bool_t reset_condition = phase >= period;
    //     // vec_t wrappedPhase = phase - period;
    //     // return select(reset_condition, wrappedPhase, phase);
    //     constexpr size_t size = simd_traits<>
    //     phase[]
        
    // }
}
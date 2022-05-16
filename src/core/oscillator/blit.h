#pragma once

#include "../constants.h"
#include "../control.h"

namespace algae::dsp::core::oscillator{


    template<typename sample_t>
    static inline size_t max_number_of_harmonics(const sample_t& period_in_samples){

        return (unsigned int) floor( 0.5 * period_in_samples);
        
    }

    template<typename sample_t>
    const inline sample_t sincM(const sample_t& phase, const sample_t& M){

        sample_t pulse;
        sample_t denominator = sin( phase );
        bool div_zero = denominator <= std::numeric_limits<sample_t>::epsilon();
        return div_zero?1:sin( M * phase )/(M*denominator);

    }

    template<typename sample_t>
    struct blit_train_t{
        sample_t phase;
    };


    template<typename sample_t>
    const inline std::pair<blit_train_t<sample_t>,sample_t> process(blit_train_t<sample_t> train, const sample_t& freq, const sample_t& sampleRate){
        const sample_t f = freq>0?freq:0.001;
        const sample_t phi = compute_phase_increment_<sample_t>(f, sampleRate, M_PI);
        const sample_t period_in_samples = sampleRate/f;
        const sample_t M = 2*max_number_of_harmonics(period_in_samples) +1;

        sample_t pulse = sincM(train.phase, M);
        
        train.phase = update_phase_<sample_t>(train.phase, phi, sample_t(M_PI));

        return std::pair(train, pulse);

    }
    


}
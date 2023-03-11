#pragma once

#include "../constants.h"
#include "../control.h"

namespace algae::dsp::core::oscillator{

    // the following is heavily inspired by STK: https://ccrma.stanford.edu/software/stk/information.html
    template<typename sample_t>
    static inline size_t max_number_of_harmonics(const sample_t& period_in_samples){

        return (unsigned int) floor( 0.5 * period_in_samples);
        
    }

    template<typename sample_t>
    const inline sample_t sincM(const sample_t& phase, const sample_t& M){

        sample_t denominator = sin( phase );
        bool div_zero = denominator <= std::numeric_limits<sample_t>::epsilon();
        return div_zero?1:sin( M * phase )/(M*denominator);

    }

    template<typename sample_t>
    struct blit_train_t{
        sample_t phase=0;
    };


    template<typename sample_t>
    const inline std::pair<blit_train_t<sample_t>,sample_t> process(blit_train_t<sample_t> train, const sample_t& freq, const sample_t& sampleRate){
        const sample_t f = freq>0?freq:0.001;
        const sample_t phi = compute_phase_increment_<sample_t>(f, sampleRate, M_PI);
        const sample_t period_in_samples = sampleRate/f;
        const sample_t M = 2*max_number_of_harmonics(period_in_samples) +1;

        sample_t pulse = sincM(train.phase, M);
        
        train.phase = update_phase_<sample_t>(train.phase, phi, sample_t(M_PI));

        return std::pair<blit_train_t<sample_t>,sample_t>(train, pulse);

    }

    template<typename sample_t>
    struct click_t{
        sample_t phase=M_PI;
    };

    template<typename sample_t>
    inline const std::pair<click_t<sample_t>, sample_t> process(
        click_t<sample_t> click, const sample_t& trig, const sample_t& width_samps=5
    ){
        sample_t phase = trig>0?0:click.phase;
        const sample_t M = 2*max_number_of_harmonics(width_samps) +1;
        const sample_t period = sample_t(M_PI);
        const sample_t phi = period/width_samps;
        bool playing = phase <= period;
        sample_t out = playing?sincM(phase, M):0;
        
        if (playing) {
            phase += phi;
            click.phase = phase;
        }

        return std::pair<click_t<sample_t>, sample_t>(click, out);
    }
    


}
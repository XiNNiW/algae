#pragma once
#include <math.h>

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::oscillator{

    template<typename sample_t, typename frequency_t>
    const sample_t sinOsc(const sample_t& phase){
        return sin(M_PI_2*phase);
    }

    template<typename sample_t>
    const sample_t noise(){
        return (sample_t) 2.0 * (((sample_t) rand() / (sample_t) RAND_MAX) - (sample_t)0.5);
    }

    template<typename frequency_t>
    struct phasor_t{
        frequency_t phase=0;
        phasor_t()=default;
        phasor_t(frequency_t phase):phase(phase){}
    };


    template<typename sample_t, typename frequency_t>
    const phasor_t<frequency_t> update_phasor(phasor_t<frequency_t> phasor_state,const frequency_t& freq, const frequency_t& sampleRate=44100.0){
        frequency_t phi = freq/sampleRate;
        frequency_t phase = phasor_state.phase;
        phase += phi;
        phase -= floor(phase);
        return phasor_t(phase);
    }

}


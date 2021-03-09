#pragma once

#include <math.h>

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::units{
    template<typename frequency_t>
    frequency_t mtof(frequency_t n){
        const frequency_t REF_FREQ = 440.0;
        const int REF_NOTE = 69;
        return  REF_FREQ * pow (2, (n - REF_NOTE) / 12);
    };

    template<typename sample_t, typename frequency_t>
    sample_t beats(sample_t n, sample_t bpm, frequency_t sampleRate=44100.0) { 
        frequency_t SECONDS_PER_MINUTE = 60;
        return (n/bpm)*SECONDS_PER_MINUTE*sampleRate;
    };

    template<typename sample_t, typename frequency_t>
    sample_t msToSamples(sample_t ms, frequency_t sampleRate=44100.0) { 
        const frequency_t MILLISECONDS_PER_SECOND = 1000.0;
        return ms*MILLISECONDS_PER_SECOND*sampleRate;
    };
}

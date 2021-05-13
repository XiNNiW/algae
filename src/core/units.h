#pragma once

#include <math.h>


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
        const frequency_t SECONDS_PER_MILLISECOND = 1/1000.0;
        return ms*sampleRate*SECONDS_PER_MILLISECOND;
    };

 // implementation borrowed from Miller Puckette's PD
    template<typename sample_t>
    const inline sample_t dbtorms(sample_t db){
        const sample_t LOGTEN = 2.302585092994046;
        if (db <= 0){
            return(0);
        }
        else
        {
            if (db > 485)
                db = 485;
        }
        return (exp((LOGTEN * 0.05) * (db-100.)));
    }
}

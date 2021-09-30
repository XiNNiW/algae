#pragma once
#include <math.h>
namespace algae::dsp::core::units {
    template<typename sample_t, typename frequency_t>
    sample_t msToSamples(sample_t ms, frequency_t sampleRate=44100.0) { 
        const frequency_t SECONDS_PER_MILLISECOND = 1/1000.0;
        return ms*sampleRate*SECONDS_PER_MILLISECOND;
    };

}
#pragma once
namespace algae::dsp::core::units {
    template<typename sample_t, typename frequency_t>
    sample_t beatsToSamples(sample_t n, sample_t bpm, frequency_t sampleRate=44100.0) { 
        frequency_t SECONDS_PER_MINUTE = 60;
        return (n/bpm)*SECONDS_PER_MINUTE*sampleRate;
    };
}
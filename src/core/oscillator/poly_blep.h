#pragma once
namespace algae::dsp::core::oscillator{
    template<typename sample_t>
    const inline sample_t blep(const sample_t& t, const sample_t& dt){
        if (t < dt) {
            auto x = (t / dt - 1);
            return -x*x;
        } else if (t > 1 - dt) {
            auto x = (t - 1) / dt + 1;
            return x*x;
        } else {
            return 0;
        }
    } 

    template<typename sample_t>
    const inline sample_t blamp(const sample_t& t, const sample_t& dt){
        if (t < dt) {
            auto x = t / dt - 1;
            return -1 / 3.0 * x * x * x;
        } else if (t > 1 - dt) {
            auto x = (t - 1) / dt + 1;
            return 1 / 3.0 * x * x * x;
        } else {
            return 0;
        }
    } 
}
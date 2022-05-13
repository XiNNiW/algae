#pragma once
#include "phasor.h"

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
        // if (t < dt) {
        //     t /= dt;
        //     return t+t - t*t - 1.0;
        // }
        // // -1 < t < 0
        // else if (t > 1.0 - dt) {
        //     t = (t - 1.0) / dt;
        //     return t*t + t+t + 1.0;
        // }
        // // 0 otherwise
        // else return 0.0;
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


    template<typename sample_t>
    struct blep_saw_t{
        sample_t phase=0;
    };

    template<typename sample_t, typename frequency_t>
    const inline std::pair<blep_saw_t<sample_t>, sample_t> process(blep_saw_t<sample_t> saw, const sample_t& phase_increment){
        sample_t out = 2*saw.phase - 1;
        out -= blep<sample_t>(saw.phase, phase_increment);
        saw.phase = update_phase<sample_t, frequency_t>(saw.phase, phase_increment);
        return std::pair<blep_saw_t<sample_t>, sample_t>(saw, out);
    }

    template<typename sample_t>
    struct blep_square_t{
        sample_t phase=0;
    };

    template<typename sample_t, typename frequency_t>
    const inline std::pair<blep_square_t<sample_t>, sample_t> process(blep_square_t<sample_t> square, const sample_t& phase_increment, const sample_t pwidth=0.5){
        sample_t out = square.phase<pwidth?1:-1;
        out += blep<sample_t>(square.phase, phase_increment);
        out -= blep<sample_t>(fmod(square.phase + (1-pwidth), 1.0), phase_increment);
        square.phase = update_phase<sample_t, frequency_t>(square.phase, phase_increment);
        return std::pair<blep_square_t<sample_t>, sample_t>(square, out);
    }

    template<typename sample_t>
    struct blep_tri_t{
        sample_t phase=0;
        sample_t y1=0;
    };

    template<typename sample_t, typename frequency_t>
    const inline std::pair<blep_tri_t<sample_t>, sample_t> process(blep_tri_t<sample_t> tri, const sample_t& phase_increment){
        const sample_t pwidth = 0.5;
        sample_t out = tri.phase<pwidth?1:-1;
        out += blep<sample_t>(tri.phase, phase_increment);
        out -= blep<sample_t>(fmod(tri.phase + (1-pwidth), 1.0), phase_increment);
        out = phase_increment * out + (1 - phase_increment) * tri.y1;
        tri.y1 = out;
        tri.phase = update_phase<sample_t, frequency_t>(tri.phase, phase_increment);
        return std::pair<blep_tri_t<sample_t>, sample_t>(tri, out);
    }
}
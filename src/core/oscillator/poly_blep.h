#pragma once
#include "phasor.h"
#include "../simd.h"

namespace algae::dsp::core::oscillator{
    using algae::dsp::core::simd::simd_traits;
    using algae::dsp::core::simd::select;
    template <class sample_t, class vec_t>
    struct poly_blep_invoker
    {
        static const inline vec_t blep(const vec_t& t, const vec_t& dt){
            auto cond1 = (t < dt);
            auto x1 = (t / dt - vec_t(1));
            auto res1 = -x1*x1;
            auto cond2 = (t > vec_t(1) - dt);
            auto x2 = (t - vec_t(1)) / dt + vec_t(1);
            auto res2 = x2*x2;

            return select(cond1,res1,select(cond2,res2,0));

        }
    };

    template <class sample_t>
    struct poly_blep_invoker<sample_t, sample_t>
    {

        static const inline sample_t blep(const sample_t& t, const sample_t& dt){
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
    };

    template<typename sample_t>
    const inline sample_t blep(const sample_t& t, const sample_t& dt){
 
        return poly_blep_invoker<typename simd_vector_traits<sample_t>::value_type,sample_t>::blep(t,dt);
        
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

    // template<typename sample_t, typename vec_t = typename simd_traits<sample_t>::type>
    // const inline vec_t blamp(
    //     const vec_t& t, const vec_t& dt
    // ){
    //     auto cond1 = (t < dt);
    //     auto cond2 = (t > 1 - dt);
     
    //     auto x1 = t / dt - 1;
    //     auto res1 = -1 / 3.0 * x1 * x1 * x1;

    //     auto x2 = (t - 1) / dt + 1;
    //     auto res2 = 1 / 3.0 * x2 * x2 * x2;

    //     return select(cond1,res1,select(cond2,res2,0));
    // }


    template<typename sample_t>
    struct blep_saw_t{
        sample_t phase=0;
    };

    template<typename sample_t, typename frequency_t>
    const inline std::pair<blep_saw_t<sample_t>, sample_t> process(blep_saw_t<sample_t> saw, const sample_t& phase_increment){
        const sample_t one = 1;
        sample_t out = sample_t(2)*saw.phase - one;
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
        const sample_t one = 1;
        sample_t out = square.phase<pwidth?one:-one;
        out += blep<sample_t>(square.phase, phase_increment);
        out -= blep<sample_t>(fmod(square.phase + (one-pwidth), one), phase_increment);
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
        const sample_t one = 1;
        const sample_t pwidth = 0.5;
        sample_t out = tri.phase<pwidth?one:-one;
        out += blep<sample_t>(tri.phase, phase_increment);
        out -= blep<sample_t>(fmod(tri.phase + (one-pwidth), one), phase_increment);
        out = phase_increment * out + (one - phase_increment) * tri.y1;
        tri.y1 = out;
        tri.phase = update_phase<sample_t, frequency_t>(tri.phase, phase_increment);
        return std::pair<blep_tri_t<sample_t>, sample_t>(tri, out);
    }
}
#pragma once
#include <math.h>
#include "../constants.h"
#include "../filter.h"
#include "cos_table_lookup.h"

namespace algae::dsp::core::oscillator {

    template<typename sample_t>
    struct fbfm_t{
        sample_t y1 = 0;

        inline const sample_t& process(
            const sample_t& carrier_phase, 
            const sample_t& modulator_phase, 
            const sample_t& index, 
            const sample_t& feedback_amt
        ){
            y1 = sin(
                TWO_PI*carrier_phase 
                + 
                index*sin(
                    TWO_PI*modulator_phase + feedback_amt*y1
                )
            );
    
            return y1;
        }
    };
    

    template<typename sample_t>
    const inline std::pair<fbfm_t<sample_t>, sample_t> process(
        fbfm_t<sample_t> osc, const sample_t& carrier_phase, const sample_t& modulator_phase, const sample_t& index, const sample_t& feedback_amt
    ){
        const sample_t output = sin(
            TWO_PI*carrier_phase 
            + 
            index*sin(
                TWO_PI*modulator_phase + feedback_amt*osc.y1
            )
        );
        osc.y1 = output;
        return std::pair<fbfm_t<sample_t>, sample_t>(osc, output);
    }

    template<typename sample_t>
    const inline fbfm_t<sample_t> process(
        fbfm_t<sample_t> osc, const sample_t* carrier_phase, const sample_t* modulator_phase, const sample_t* index, const sample_t* feedback_amt,
        const size_t& block_size,
        // -->
        sample_t* out
    ){
        sample_t carrier[block_size];
        block_mult(carrier_phase,TWO_PI,block_size, carrier);
        sample_t modulator[block_size];
        block_mult(modulator_phase,TWO_PI,block_size, modulator);
        for(size_t idx=0; idx<block_size; idx++) {
            out[idx] = sin(
                TWO_PI*carrier_phase 
                + 
                index*sin(
                    TWO_PI*modulator_phase + feedback_amt*osc.y1
                )
            );
            osc.y1 = out[idx]; 
        }

        return osc;
    }

    using algae::dsp::core::filter::onepole_t;
    using algae::dsp::core::filter::lop;


    template<typename sample_t>
    struct lp_fbfm_t{
        sample_t y1;
        onepole_t<sample_t> lp; 
    };


    template<typename sample_t, typename frequency_t>
    const inline lp_fbfm_t<sample_t> lp_fbfm(lp_fbfm_t<sample_t> osc, const frequency_t& cutoff, const frequency_t& sampleRate){
        osc.lp = lop<sample_t, frequency_t>(osc.lp, cutoff, sampleRate);
        return osc;
    } 
    
    template<typename sample_t>
    const inline std::pair<lp_fbfm_t<sample_t>, sample_t> process(
        lp_fbfm_t<sample_t> osc, const sample_t& carrier_phase, const sample_t& modulator_phase, const sample_t& index, const sample_t& feedback_amt
    ){
        osc.lp = process<sample_t>(osc.lp,osc.y1);
        const sample_t output = osc.y1 = sin(
            TWO_PI*carrier_phase 
            + 
            index*sin(
                TWO_PI*modulator_phase + feedback_amt*osc.lp.y1
            )
        );
        return std::pair<lp_fbfm_t<sample_t>, sample_t>(osc, output);
    }


    template<typename sample_t>
    struct sin_fb_t {
        sample_t y1=0;
        sample_t phase=0;

        const inline sample_t process(
            const sample_t& carrierFreq, const sample_t& phaseMod, const sample_t& feedback, const sample_t& sampleRate
        ){
            const sample_t out = sin(phase+phaseMod+y1*feedback);
            y1 = out;

            sample_t phi = compute_phase_increment_<sample_t>(carrierFreq, sampleRate, TWO_PI);
            phase = update_phase_<sample_t>(phase, phi, TWO_PI);
            return out;
        }
    };

    template<typename sample_t>
    const inline std::pair<sin_fb_t<sample_t>, sample_t> process(
        sin_fb_t<sample_t> osc, const sample_t& carrierFreq, const sample_t& phaseMod, const sample_t& feedback, const sample_t& sampleRate
    ){
        const sample_t out = sin(osc.phase+phaseMod+osc.y1*feedback);
        osc.y1 = out;

        sample_t phi = compute_phase_increment_<sample_t>(carrierFreq, sampleRate, TWO_PI);
        osc.phase = update_phase_<sample_t>(osc.phase, phi, TWO_PI);
        return std::pair<sin_fb_t<sample_t>, sample_t>(osc, out);
    }


}
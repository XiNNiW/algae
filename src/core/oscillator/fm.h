#pragma once
#include <math.h>
#include "../constants.h"
#include "../filter.h"
#include "cos_table_lookup.h"

namespace algae::dsp::core::oscillator {

    template<typename sample_t>
    struct fbfm_t{
        sample_t y1 = 0;
    };
    

    template<typename sample_t>
    const inline std::pair<fbfm_t<sample_t>, sample_t> process(
        fbfm_t<sample_t> osc, const sample_t& carrier_phase, const sample_t& modulator_phase, const sample_t& index, const sample_t& feedback_amt
    ){
        const sample_t output = osc.y1 = sin(
            TWO_PI*carrier_phase 
            + 
            index*sin(
                TWO_PI*modulator_phase + feedback_amt*osc.y1
            )
        );
        return std::pair(osc, output);
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
        return std::pair(osc, output);
    }


}
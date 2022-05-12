#pragma once
#include "../audio_block.h"
#include "../math_tools.h"
#include "../control.h"
#include "fm.h"
#include "phasor.h"
#include "noise.h"
#include "../spacialization.h"

namespace algae::dsp::core::oscillator {

    template<typename sample_t>
    struct fm_grain_t{
        sample_t grain_phase=1;
        sample_t carrier_phase=0;
        sample_t modulator_phase=0;
        fbfm_t<sample_t> osc;
        sample_t grain_freq=0.1;
        sample_t carrier_freq=0.1;
        sample_t mod_freq=0.1;
        sample_t fm_index=0.1;
        sample_t feedback=0.1;
        sample_t pan_position=0.5;
    };

    template<typename sample_t>
    void random_triggers(
        const sample_t* prob, const size_t& blocksize, 
        //-->
        bool* triggers
    ){
        
        for(size_t idx=0; idx<blocksize; idx++){
           triggers[idx] = prob[idx]>( static_cast<sample_t>(rand()) /  static_cast<sample_t>(RAND_MAX));
        }
    }

    template<typename sample_t>
    void random_triggers(
        const sample_t* prob, const size_t& blocksize, 
        const sample_t* noise,
        //-->
        bool* triggers
    ){
        
        for(size_t idx=0; idx<blocksize; idx++){
           triggers[idx] = prob[idx]>noise[idx];
        }
    }

    using algae::dsp::core::spacialization::pan;
    template<typename sample_t, const sample_t (* window_fn)(const sample_t&)>
    const fm_grain_t<sample_t> process(
        fm_grain_t<sample_t>grain, const sample_t* grainFreq,
        const sample_t* carrier_freq, const sample_t* mod_freq, const sample_t* index, const sample_t* feedback, 
        const sample_t* pan_position,
        const bool* triggers,
        const size_t& blocksize, const sample_t& sample_rate,
        //-->
        sample_t* out_left,
        sample_t* out_right
    ){   
        // auto gfreq      = grainFreq[0]>0?grainFreq[0]:0.01;
        // auto cfreq      = carrier_freq[0];
        // auto mfreq      = mod_freq[0];
        // auto fmindex    = index[0];
        // auto fb         = feedback[0];
        // auto position   = pan_position[0];
        
        for(size_t idx=0; idx<blocksize; idx++){
            bool active = grain.grain_phase < 1;
            if (triggers[idx] || active){

                if(triggers[idx] && (!active)){
                    grain.grain_freq        = grainFreq[idx]>1?grainFreq[idx]:1;
                    grain.carrier_freq      = carrier_freq[idx];
                    grain.mod_freq          = mod_freq[idx];
                    grain.fm_index          = index[idx];
                    grain.feedback          = feedback[idx];
                    grain.pan_position      = pan_position[idx];
                    grain.grain_phase       = 0;
                    // grain.carrier_phase = 0;
                    // grain.modulator_phase = 0;
                    // grain.osc.y1 = 0;
                }

                sample_t out;
                std::tie(grain.osc, out) = process<sample_t>(grain.osc, grain.carrier_phase, grain.modulator_phase, grain.fm_index, grain.feedback);
                out *= window_fn(grain.grain_phase);
                std::tie(out_left[idx], out_right[idx]) = pan<sample_t, 1024>(out, grain.pan_position);
               
                grain.grain_phase       += compute_phase_increment<sample_t>(grain.grain_freq, sample_rate);
                grain.carrier_phase     =  update_phase<sample_t>(grain.carrier_phase,   compute_phase_increment<sample_t>(grain.carrier_freq,sample_rate));
                grain.modulator_phase   =  update_phase<sample_t>(grain.modulator_phase, compute_phase_increment<sample_t>(grain.mod_freq,sample_rate));
            
            }else{
                out_left[idx]=0;
                out_right[idx]=0;
            }
            
        }
        
        return grain;
    }

}
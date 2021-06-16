#pragma once

#include <iostream>

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::control{

    template<typename sample_t>
    struct samphold{
        sample_t trigger;
        sample_t value;
    };

    template<typename sample_t>
    const samphold<sample_t> update_samphold(const samphold<sample_t>& state, sample_t value, sample_t trigger){
        samphold<sample_t> next;
        if(trigger < state.trigger){
            next.value = value;
        }
        next.trigger = trigger;
        return next;
    }

    template<typename sample_t>
    struct accum_t{
        sample_t value;
    };

    template<typename sample_t>
    accum_t<sample_t> update_accum(accum_t<sample_t> state, sample_t amount=1){
        return accum_t{state.value+amount};
    }

    template<typename sample_t>
    struct ramp_t {
        long index=0;
        sample_t value=0;
    };

    template<typename sample_t>
    const ramp_t<sample_t> update_ramp(const ramp_t<sample_t>& state, const sample_t& start, const sample_t& end, const long& ramptime_in_samples, const long& offset=0){
        
        long index = ((state.index-offset)>0)?state.index-offset:0;
        long r = ramptime_in_samples>0? ramptime_in_samples : 1;
        sample_t phase = (((sample_t)index)/((sample_t)r));
        return  
            (index<=ramptime_in_samples)
            ? ramp_t<sample_t>{ state.index+1, static_cast<sample_t>(start*(1.0-phase)+end*phase)}
            : ramp_t<sample_t>{ state.index, end};

    }

    template<typename sample_t>
    const ramp_t<sample_t> reset_ramp(const ramp_t<sample_t>& state,const sample_t& start){
        return ramp_t<sample_t>{start,0};
    }

    template<typename sample_t>
    const ramp_t<sample_t> update_ad(
        ramp_t<sample_t> state,
        int a,
        int d
    ){
        return update_ad<sample_t>(state,0.0,a,1.0,d,0.0);
                
    }

    template<typename sample_t>
    const ramp_t<sample_t> update_ad(
        ramp_t<sample_t> state,
        sample_t initial,
        int a,
        sample_t peak,
        int d,
        sample_t final
    ){
        return  (state.index<=a)
                ? update_ramp<sample_t>(state,initial,peak,a)
                : update_ramp<sample_t>(state,peak,final,d,a);
                
    }

    template<typename sample_t>
    struct adsr_t{
        ramp_t<sample_t> env;
        sample_t last;
    };

    template<typename sample_t>
    const inline adsr_t<sample_t> update_adsr(
        const adsr_t<sample_t>& state, 
        const bool& trig, 
        const int& attack_time_in_samples, 
        const int& decay_time_in_samples, 
        const sample_t& sustain_level, 
        const int& release_time_in_samples
    )
    {
        const sample_t& a = attack_time_in_samples;
        const sample_t& d = decay_time_in_samples;
        const sample_t& r = release_time_in_samples;
        
        if(trig){
            ramp_t<sample_t> ramp = update_ad<sample_t>(state.env,0,a,1.0,d,sustain_level);
            return adsr_t<sample_t>{
                ramp,
                ramp.value
            };
        } else {
            
            return 
                
                state.env.index>0?
                adsr_t<sample_t>{
                    update_ramp<sample_t>(
                        state.env,
                        state.last,
                        0.0,
                        r,
                        a+d
                    ),
                    state.last
                }
                :state;
               
        }

    }

    template<typename sample_t>
    const ramp_t<sample_t> inline computeRampForEnvPhase(ramp_t<sample_t> state, bool trig, int offset){
        long index = (trig&&!state.trig)
                    ? 0
                    :(state.index>=offset)
                    ? state.index-offset
                    : 0;
        return ramp_t<sample_t>{trig, index, state.value};
    }

}

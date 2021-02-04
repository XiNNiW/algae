#pragma once

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
        bool trig;
        long index;
        sample_t value;
    };

    template<typename sample_t>
    const ramp_t<sample_t> update_ramp(const ramp_t<sample_t>& state, const bool& trig, const sample_t& start, const sample_t& end, const long& ramptime_in_samples, const long& offset=0){
        long index = (trig&&!state.trig)
                    ? 0
                    :(state.index>=offset)
                    ? state.index-offset
                    : 0;
        sample_t phase = (((sample_t)index)/((sample_t)ramptime_in_samples));
        sample_t value = 
            (index<ramptime_in_samples)
            ? start*(1.0-phase)+end*phase
            : end;
        return ramp_t<sample_t>{trig,state.index+1,value};
    }

    template<typename sample_t>
    const ramp_t<sample_t> reset_ramp(const ramp_t<sample_t>& state,const sample_t& start){
        return ramp_t<sample_t>{false,start,0};
    }

    template<typename sample_t,typename frequency_t>
    const ramp_t<sample_t> update_adsr(
        const ramp_t<sample_t>& state, 
        const bool& trig, 
        const int& attack_time_in_samples, 
        const int& decay_time_in_samples, 
        const sample_t& sustain_level, 
        const int& release_time_in_samples, 
        const frequency_t& sampleRate=44100.0
    )
    {
        if(trig){
            ramp_t<sample_t> next = 
                (state.index < attack_time_in_samples)
                ? update_ramp(state, trig, state.value, 1.0, attack_time_in_samples-state.index)
                : (state.index < attack_time_in_samples + decay_time_in_samples)
                ? update_ramp(state, trig, 1.0, sustain_level, decay_time_in_samples, attack_time_in_samples)
                : ramp_t<sample_t>{trig, state.index, sustain_level};
            return next;
        } else if(state.value == 0){
            return state;
        } else {
            ramp_t<sample_t> next = 
                (state.index < attack_time_in_samples + decay_time_in_samples + release_time_in_samples)
                ? update_ramp(state, trig, sustain_level, 0.0, attack_time_in_samples+decay_time_in_samples)
                : ramp_t<sample_t>{false,0,0};
            return next;
        }

    }

}

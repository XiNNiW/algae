#pragma once
// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::control {
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
}
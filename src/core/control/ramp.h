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

    template<typename sample_t>
    struct perc_t{
        sample_t phase = 3;
    }; 

    template<typename sample_t>
    const inline std::pair<perc_t<sample_t>, sample_t> process(
        perc_t<sample_t> env, 
        const sample_t& trig, 
        const sample_t& attack_in_samps,
        const sample_t& decay_in_samps,
        const sample_t& curve
    ){
        sample_t phase = trig>0?0:env.phase;
        if(phase<1){
            
            const sample_t phi = attack_in_samps>0?1.0/attack_in_samps:1;

            sample_t out = pow(curve, 1-phase);

            phase += phi;

            env.phase = phase;
            return std::pair(env, out);
        } else if (phase<=2) {
            const sample_t phi = 1.0/decay_in_samps;

            sample_t out = pow(curve, phase-1);

            phase += phi;

            env.phase = phase;
            return std::pair(env, out);
        }
        else {
            return std::pair(env, 0);
        }

    }
}
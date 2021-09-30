#pragma once 
namespace algae::dsp::core::control {
    template<typename sample_t>
    struct adsr_params_t {
        size_t attack=20;
        size_t decay=150;
        sample_t sustain=0.77;
        size_t release=100;
    };

    template<typename sample_t>
    struct adsr_t{
        ramp_t<sample_t> env;
        sample_t last=0;
        static inline const adsr_t<sample_t> process( 
            const adsr_t<sample_t>& state, 
            const bool& trig, 
            const adsr_params_t<sample_t>& p
        ){
            const sample_t& a = p.attack;
            const sample_t& d = p.decay;
            const sample_t& s = p.sustain;
            const sample_t& r = p.release;
            
            if(trig){
                ramp_t<sample_t> ramp = update_ad<sample_t>(state.env,0,a,1.0,d,s);
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
}
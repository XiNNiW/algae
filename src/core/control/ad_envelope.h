#pragma once 
namespace algae::dsp::core::control {
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
}
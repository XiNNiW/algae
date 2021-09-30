#pragma once

namespace algae::dsp::core::control {
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
}
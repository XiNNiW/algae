#pragma once

namespace algae::dsp::core::control {
    template<typename sample_t>
    struct accum_t{
        sample_t value;
    };

    template<typename sample_t>
    accum_t<sample_t> update_accum(accum_t<sample_t> state, sample_t amount=1){
        return accum_t{state.value+amount};
    }
}
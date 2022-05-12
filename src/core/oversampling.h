#pragma once

namespace algae::dsp::core::oversampling {

    template<typename sample_t>
    void upsample_zero_pad(const sample_t* in, const size_t& block_size, const size_t& upsample_factor, sample_t* out){
        for(size_t idx=0; idx<block_size; idx++){
            size_t group_index = idx*upsample_factor;
            out[group_index] = in[idx];
            for(size_t zero_idx=1; zero_idx<upsample_factor; zero_idx++)
                out[group_index+zero_idx] = 0;
        }
    }

}
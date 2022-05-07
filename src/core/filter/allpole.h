#pragma once
#include "../audio_block.h"

namespace algae::dsp::core::filter {

    template<typename sample_t, size_t ORDER, size_t SIZE = ORDER-1>
    struct allpole_t {
        sample_t input_gain = 1;
        std::array<sample_t, ORDER> as;
        std::array<sample_t, ORDER> ys;
    };

    template<typename sample_t, size_t ORDER, size_t SIZE = ORDER-1>
    const inline allpole_t<sample_t, ORDER> allpole(const std::array<sample_t, ORDER> as){
        allpole_t<sample_t, ORDER> filter;
        for(size_t idx=0; idx<SIZE; idx++){
            filter.ys[idx]=0;
            filter.as[idx]=as[idx+1]/as[0];
        }
        filter.input_gain = 1/as[0];
        return filter;
    }

    template<typename sample_t, size_t ORDER, size_t SIZE = ORDER-1> 
    const inline allpole_t<sample_t, ORDER> process(allpole_t<sample_t, ORDER> filter, const sample_t& input) {
      
        sample_t output = input*filter.input_gain;
      
        for(size_t idx = SIZE; idx --> 0;){

            output -= filter.ys[idx]*filter.as[idx];
            filter.ys[idx] = idx>0?filter.ys[idx-1]:output;
            
        }
      
        return filter;

    }

    template<typename sample_t, size_t BLOCK_SIZE, size_t ORDER>
    const inline std::pair<allpole_t<sample_t, ORDER>, AudioBlock<sample_t, BLOCK_SIZE>> process(
        allpole_t<sample_t, ORDER> filter,
        const AudioBlock<sample_t, BLOCK_SIZE>& input
    ){
        AudioBlock<sample_t, BLOCK_SIZE> output;
        for(size_t idx=0; idx<BLOCK_SIZE; idx++){
            filter = process<sample_t, ORDER>(filter, input[idx]);

            output[idx] = filter.ys[0];
        }
        
        return std::pair(filter,output);
    }



}
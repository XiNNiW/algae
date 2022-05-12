#pragma once
#include "math.h"
#include "audio_block.h"
#include "oscillator.h"

namespace algae::dsp::core::spacialization {

    using algae::dsp::core::oscillator::cos_t;

    template<typename sample_t, size_t TABLE_SIZE>
    const inline std::pair<sample_t, sample_t> pan(
        const sample_t& signal, const sample_t& position
    ){

        // const sample_t center = 0.125;
        sample_t power_left = 0.25*position;

        sample_t power_right = power_left + 0.75; 
        
        return std::pair<sample_t, sample_t>(
            signal*cos(TWO_PI*power_left), //cos_t<sample_t, TABLE_SIZE>::lookup(power_left), 
            signal*cos(TWO_PI*power_right)  //cos_t<sample_t, TABLE_SIZE>::lookup(power_right)
        );
    
    }

    // template<typename sample_t, size_t TABLE_SIZE>
    // void pan(
    //     const sample_t* signal, const sample_t* position,
    //     const size_t& blocksize,
    //     //->
    //     const sample_t* out_left,
    //     const sample_t* out_right,
    // ){

    //     const sample_t center = 0.125;
    //     sample_t power_left = 0.25*position;

    //     power_left = lerp<sample_t>(power_left,center,0);

    //     sample_t power_right = power_left + 0.75;
       
    //     block_mult(
    //         signal, cos_t<sample_t, TABLE_SIZE>::lookup(power_left),blocksize, 
    //         out_left
    //     );
    //     block_mult(
    //         signal, cos_t<sample_t, TABLE_SIZE>::lookup(power_right),blocksize, 
    //         out_right
    //     );
        
    // }

}
#pragma once
#include <math.h>
#include "../audio_block.h"
namespace algae::dsp::core::oscillator {
   template<typename sample_t>
    struct stk_blit_square_t{
        int m=0;
        sample_t phase_increment=0;
        sample_t phase=0;
        sample_t p=0;
        sample_t a=0;
        sample_t state=0;
        sample_t last_blit_output=0;
        sample_t dc_blocker_x1=0;
    };

    template<typename sample_t, typename frequency_t>
    stk_blit_square_t<sample_t> setFrequency(stk_blit_square_t<sample_t> square, frequency_t frequency, frequency_t sampleRate){

        square.p = 0.5*sampleRate/frequency;
        square.phase_increment = M_PI/square.p;

        int max_harmonics = floor(0.5*square.p);
        square.m = 2*(max_harmonics+1);
        square.a = square.m / square.p;

        return square;
    }
    
    template<typename sample_t, typename frequency_t>
    const inline stk_blit_square_t<sample_t> stk_blit_square(const frequency_t& frequency, const frequency_t& sampleRate){
        stk_blit_square_t<sample_t> square;

        square = setFrequency(square, frequency, sampleRate);
        // square.state = -0.5 * square.a;
        return square;
    }

    template<typename sample_t>
    const inline stk_blit_square_t<sample_t> process(stk_blit_square_t<sample_t> sq){
        
        sample_t tmp = sq.last_blit_output;
        sample_t denominator = sin( sq.phase );
        if ( fabs(denominator) < std::numeric_limits<sample_t>::epsilon() )
            if ( sq.phase < 0.1 || sq.phase > TWO_PI - 0.1 )
                sq.last_blit_output = sq.a;
            else
                sq.last_blit_output = -sq.a;
        else {
            sq.last_blit_output =  sin( sq.m * sq.phase );
            sq.last_blit_output /= sq.p * denominator;
        }

        sq.last_blit_output += tmp;
        sq.state = sq.last_blit_output - sq.dc_blocker_x1 + 0.999*sq.state;

        sq.dc_blocker_x1 = sq.last_blit_output;

        sq.phase += sq.phase_increment;
        if ( sq.phase >= TWO_PI ) sq.phase -= TWO_PI;
        // if ( sq.phase >= TWO_PI ) sq.phase -= TWO_PI;
            
        return sq;
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const inline std::pair<stk_blit_square_t<sample_t>,AudioBlock<sample_t,BLOCKSIZE>> process(stk_blit_square_t<sample_t> sq) {
        
        AudioBlock<sample_t,BLOCKSIZE> output;
        for(size_t idx=0;idx<BLOCKSIZE;idx++){
            sq = process<sample_t>(sq);
            output[idx] = sq.state;
        }
        
        return std::pair(sq,output);
    }
}
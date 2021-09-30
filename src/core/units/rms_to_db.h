#pragma once
#include <math.h>
#include "../constants.h"
namespace algae::dsp::core::units{
    //Implementation borrowed from Miller Puckette's Pure Data
    template<typename sample_t>
    const inline sample_t rmstodb(sample_t rms){

        if (rms <= 0) return (0);
        else
        {
            sample_t val = 100 + 20./LOGTEN * log(rms);
            return (val < 0 ? 0 : val);
        }
    
    }

}
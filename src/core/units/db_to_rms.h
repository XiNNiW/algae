#pragma once
#include <math.h>
#include "../constants.h"
namespace algae::dsp::core::units{
    // implementation borrowed from Miller Puckette's Pure Data project
    template<typename sample_t>
    const inline sample_t dbtorms(sample_t db){
        if (db <= 0){
            return(0);
        }
        else
        {
            if (db > 485)
                db = 485;
        }
        return (exp((LOGTEN * 0.05) * (db-100.)));
    }

}
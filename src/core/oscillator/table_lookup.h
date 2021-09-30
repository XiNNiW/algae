#pragma once
#include <math.h>
#include <array>
namespace algae::dsp::core::oscillator {
    template<typename sample_t, int TABLE_SIZE>
    const inline sample_t table_lookup_lin_interp(const std::array<sample_t,TABLE_SIZE>& table, const sample_t& phase){
    
        sample_t _phase = phase-floor(phase);
        _phase = _phase<0 ? _phase+1 : _phase; 
        const sample_t position = _phase*(TABLE_SIZE-1);
        const int index = floor(position);
        const int x0 = index;
        int x1 = index+1;
        x1 = x1>=TABLE_SIZE?0:x1;
        const sample_t mantissa = position - index;
        const sample_t value = table[x0] + mantissa*(table[x1] - table[x0]);
        return value;
    }
}
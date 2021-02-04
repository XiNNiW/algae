#pragma once 

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::fx{

    template<typename sample_t>
    sample_t bitcrush(const sample_t& x, const sample_t& bits){
        sample_t n = pow(2,bits);
        return round(x*n)/n;
    }

    template<typename sample_t>
    struct delayline_t {
        size_t length;
        size_t currentOffset;
        sample_t *buffer;
    };

    template<typename sample_t>
    void delaywrite(delayline_t<sample_t> &state, const sample_t& input){
        sample_t *buffer = state.buffer;
        size_t w = state.currentOffset;
        size_t l = state.length;
        buffer[w++] = input;
        if (w >= l) { w -= l; }
        state.currentOffset = w;
    }

    template<typename sample_t>
    const sample_t delayread(const delayline_t<sample_t>& state, const size_t& delaytime_in_samples){
        sample_t *buffer = state.buffer;
        int l = state.length;
        l = (l > 0) ? l : 1;
        int w = state.currentOffset;
        int d = delaytime_in_samples;
        d = (0 < d && d < l) ? d : 0;
        int r = w - d;
        r = r < 0 ? r + l : r;
        return buffer[r];
    }

    template<typename sample_t>
    const sample_t delread2(const delayline_t<sample_t>& state, const sample_t& delaytime_in_ms, const sample_t& sampleRate = 44100.0) {
        sample_t *buffer = state.buffer;
        int l = state.length;
        l = (l > 0) ? l : 1;
        int w = state.currentOffset;
        sample_t d = delaytime_in_ms / (sample_t)1000 * sampleRate;
        int d0 = floor(d);
        int d1 = d0 + 1;
        sample_t t = d - d0;
        d0 = (0 < d0 && d0 < l) ? d0 : 0;
        d1 = (0 < d1 && d1 < l) ? d1 : d0;
        int r0 = w - d0;
        int r1 = w - d1;
        r0 = r0 < 0 ? r0 + l : r0;
        r1 = r1 < 0 ? r1 + l : r1;
        sample_t y0 = buffer[r0];
        sample_t y1 = buffer[r1];
        return (1 - t) * y0 + t * y1;
    }

    template<typename sample_t>
    const sample_t& delread4(const delayline_t<sample_t>& state, const sample_t& delaytime_in_ms, const sample_t& sampleRate = 44100.0) {
        sample_t *buffer = state.buffer;

        int l = state.length;
        l = (l > 0) ? l : 1;
        int w = state.woffset;
        sample_t d = delaytime_in_ms / (sample_t)1000 * sampleRate;
        int d1 = floor(d);
        int d0 = d1 - 1;
        int d2 = d1 + 1;
        int d3 = d1 + 2;
        sample_t t = d - d1;
        d0 = (0 < d0 && d0 < l) ? d0 : 0;
        d1 = (0 < d1 && d1 < l) ? d1 : d0;
        d2 = (0 < d2 && d2 < l) ? d2 : d1;
        d3 = (0 < d3 && d3 < l) ? d3 : d2;
        int r0 = w - d0;
        int r1 = w - d1;
        int r2 = w - d2;
        int r3 = w - d3;
        r0 = r0 < 0 ? r0 + l : r0;
        r1 = r1 < 0 ? r1 + l : r1;
        r2 = r2 < 0 ? r2 + l : r2;
        r3 = r3 < 0 ? r3 + l : r3;
        sample_t y0 = buffer[r0];
        sample_t y1 = buffer[r1];
        sample_t y2 = buffer[r2];
        sample_t y3 = buffer[r3];
        sample_t a0 = -t*t*t + 2*t*t - t;
        sample_t a1 = 3*t*t*t - 5*t*t + 2;
        sample_t a2 = -3*t*t*t + 4*t*t + t;
        sample_t a3 = t*t*t - t*t;
        return (a0 * y0 + a1 * y1 + a2 * y2 + a3 * y3) / 2;
    }

}

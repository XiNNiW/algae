#pragma once
#include <math.h>
#include "math_functions.h"
#include <iostream>

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::filter{
// http://musicdsp.org/files/Audio-EQ-Cookbook.txt

    using algae::dsp::core::math::clamp;

    template<typename sample_t, typename frequency_t>
    struct biquad_t {
        frequency_t b0, b1, b2, a1, a2, y1, y2;
        sample_t x1, x2;
    };

    template<typename sample_t, typename frequency_t>
    const biquad_t<sample_t,frequency_t> lowpass(
        const biquad_t<sample_t,frequency_t>& state,
        const frequency_t& cutoff,
        const sample_t& q,
        const frequency_t& sampleRate=44100.0 
    ){
        frequency_t w0 = cutoff * M_2_PI / sampleRate;
        frequency_t a = fabs(sin(w0) / (2 * q));
        frequency_t c = cos(w0);
        frequency_t b0 = (1 - c) / 2, b1 = 1 - c, b2 = (1 - c) / 2;
        frequency_t a0 = 1 + a, a1 = -2 * c, a2 = 1 - a;

        
        b0 = b0 / a0;
        b1 = b1 / a0;
        b2 = b2 / a0;
        a1 = a1 / a0;
        a2 = a2 / a0;


        return biquad_t<sample_t,frequency_t>{b0,b1,b2,a1,a2,state.y1,state.y2,state.x1,state.x2};
    }

    template<typename sample_t, typename frequency_t>
    const biquad_t<sample_t,frequency_t> highpass(
        const biquad_t<sample_t,frequency_t>& state,
        const sample_t& cutoff,
        const sample_t& q,
        const frequency_t sampleRate
    ){
        frequency_t w0 = cutoff * M_PI_2 / sampleRate;
        frequency_t a = fabs(sin(w0)/(2*q));
        frequency_t c = cos(w0);
        frequency_t b0 = (1 + c) / 2;
        frequency_t b1 = -(1 + c);
        frequency_t b2 = (1 + c) / 2;
        frequency_t a0 = 1 + a;
        frequency_t a1 = -2 * c;
        frequency_t a2 = 1 - a;
        
        b0 = b0 / a0;
        b1 = b1 / a0;
        b2 = b2 / a0;
        a1 = a1 / a0;
        a2 = a2 / a0;
        return biquad_t<sample_t,frequency_t>{b0,b1,b2,a1,a2,state.y1,state.y2,state.x1,state.x2};
        
    }

    template<typename sample_t, typename frequency_t>
    const biquad_t<sample_t,frequency_t> bandpass(
        const biquad_t<sample_t,frequency_t>& state,
        const sample_t& cutoff,
        const sample_t& q,
        const frequency_t sampleRate
    ){
        frequency_t w0 = cutoff * M_PI_2 / sampleRate;
        frequency_t a = fabs(sin(w0) / (2 * q));
        frequency_t c = cos(w0);
        frequency_t b0 = a;
        frequency_t b1 = 0;
        frequency_t b2 = -a;
        frequency_t a0 = 1 + a;
        frequency_t a1 = -2 * c;
        frequency_t a2 = 1 - a;
        b0 = b0 / a0;
        b1 = b1 / a0;
        b2 = b2 / a0;
        a1 = a1 / a0;
        a2 = a2 / a0;
        return biquad_t<sample_t,frequency_t>{b0,b1,b2,a1,a2,state.y1,state.y2,state.x1,state.x2};
        
    }


    template<typename sample_t, typename frequency_t>
    const biquad_t<sample_t,frequency_t> resonant_fail(
        const biquad_t<sample_t,frequency_t>& state,
        const sample_t& centerFrequency,
        const sample_t& q,
        const sample_t& g,
        const frequency_t sampleRate=44100
    )
    {  
        const sample_t& fc = centerFrequency;
        const frequency_t& w = fc/sampleRate;
        const frequency_t& R = exp(-M_PI*w/q);

        const frequency_t& b0 = g;
        const frequency_t& b1 = 0;
        const frequency_t& b2 = -g*R;
        const frequency_t& a1 = -2*R*cos(M_2_PI*w);
        const frequency_t& a2 = R*R;

        //update_biquad evaluates yn = b0*xn+b1*xn1+b2*xn2-a1*y1-a2*yn2
        // target is yn = g*xn+-R*xn2+2R*y1-a2*yn2
        return biquad_t<sample_t,frequency_t>{b0,b1,b2,a1,a2,state.y1,state.y2,state.x1,state.x2};

    }

    template<typename sample_t, typename frequency_t>
    const biquad_t<sample_t,frequency_t> notch(
        const biquad_t<sample_t,frequency_t>& state,
        const sample_t& cutoff,
        const sample_t& q,
        const frequency_t sampleRate
    ){
        frequency_t w0 = cutoff * M_PI_2 / sampleRate;
        frequency_t a = fabs(sin(w0) / (2 * q));
        frequency_t c = cos(w0);
        frequency_t b0 = 1;
        frequency_t b1 = -2 * c;
        frequency_t b2 = 1;
        frequency_t a0 = 1 + a;
        frequency_t a1 = -2 * c;
        frequency_t a2 = 1 - a;
        b0 = b0 / a0;
        b1 = b1 / a0;
        b2 = b2 / a0;
        a1 = a1 / a0;
        a2 = a2 / a0;
        return biquad_t<sample_t,frequency_t>{b0,b1,b2,a1,a2,state.y1,state.y2,state.x1,state.x2};
        
    }

    template<typename sample_t, typename frequency_t>
    const biquad_t<sample_t,frequency_t> update_biquad(const biquad_t<sample_t,frequency_t>& state, const sample_t& x0){

        frequency_t b0 = state.b0;
        frequency_t b1 = state.b1;
        frequency_t b2 = state.b2;
        frequency_t a1 = state.a1;
        frequency_t a2 = state.a2;
        sample_t x1 = state.x1;
        sample_t x2 = state.x2;
        frequency_t y1 = state.y1;
        frequency_t y2 = state.y2;
        frequency_t y0 = b0 * x0 + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
        


        return biquad_t<sample_t,frequency_t>{b0,b1,b2,a1,a2,y0,y1,x0,x1};
    }


    template<typename sample_t>
    struct vcf_t{
        sample_t re, im;
    };

    template<typename sample_t, typename frequency_t>
    const vcf_t<sample_t> update_vcf(
        const vcf_t<sample_t>& state, 
        const sample_t& x, 
        const frequency_t& cutoff, 
        const sample_t& q, 
        const frequency_t sampleRate=44100.0)
    {
        frequency_t qinv = q > 0 ? 1 / q : 0;
        frequency_t ampcorrect = 2 - 2 / (q + 2);
        frequency_t cf = cutoff * M_PI_2 / sampleRate;
        if (cf < 0) { cf = 0; }
        frequency_t r = qinv > 0 ? 1 - cf * qinv : 0;
        if (r < 0) { r = 0; }
        frequency_t oneminusr = 1 - r;
        frequency_t cre = r * cos(cf);
        frequency_t cim = r * sin(cf);
        frequency_t re2 = state.re;
        frequency_t re = ampcorrect * oneminusr * x + cre * re2 - cim * state.im;
        frequency_t im = cim * re2 + cre * state.im;
        return vcf_t<sample_t>{re,im};
    }

    template<typename sample_t,typename frequency_t>
    struct onepole_t {
        sample_t x;
        frequency_t y;
    };

    template<typename sample_t,typename frequency_t>
    const onepole_t<sample_t,frequency_t> update_onepole_lop(
        const onepole_t<sample_t,frequency_t>& state, 
        const sample_t& x, 
        const sample_t& cutoff, 
        const frequency_t& sampleRate=44100.0
    ){
        const frequency_t w = M_PI_2 * clamp(fabs(cutoff / sampleRate), 0, (sample_t)0.5);
        const frequency_t a = (1 - sin(w)) / cos(w);
        const frequency_t b = (1 - a) / 2;
        const frequency_t y = b * (x + state.x) + a * state.y;

        return onepole_t{x,y};
    }

    template<typename sample_t,typename frequency_t>
    const onepole_t<sample_t,frequency_t> update_onepole_hip(
        const onepole_t<sample_t,frequency_t>& state,
        const sample_t& x, const sample_t& cutoff,
        const frequency_t& sampleRate=44100.0
    ){
        const frequency_t w = M_PI_2 * clamp<sample_t,frequency_t>(fabs(cutoff / sampleRate), 0, (sample_t)0.5);
        const frequency_t a = (1 - sin(w)) / cos(w);
        const frequency_t b = (1 + a) / 2;
        const frequency_t y = b * (x - state.x) + a * state.y;

        return onepole_t<sample_t,frequency_t>{x,y};
    }

    template<typename sample_t,typename frequency_t>
    struct moog_t {
        frequency_t ya1;
        frequency_t wa1;
        frequency_t yb1;
        frequency_t wb1;
        frequency_t yc1;
        frequency_t wc1;
        frequency_t yd1;
    };

    template<typename sample_t, typename frequency_t>
    const moog_t<sample_t,frequency_t> update_moog(
        const moog_t<sample_t,frequency_t>& state,
        const sample_t& audio,
        const sample_t& frequency,
        const sample_t resonance,
        const frequency_t sampleRate=44100.0
    ){

        frequency_t ya1 = state.ya1;
        frequency_t wa1 = state.wa1;
        frequency_t yb1 = state.yb1;
        frequency_t wb1 = state.wb1;
        frequency_t yc1 = state.yc1;
        frequency_t wc1 = state.wc1;
        frequency_t yd1 = state.yd1;
        frequency_t v = 2;
        frequency_t x = audio;
        frequency_t f = frequency;
        frequency_t r = resonance;
        frequency_t g = 1 - exp(-M_PI_2 * f / sampleRate);
        frequency_t ya = ya1 + v * g * tanh((x - 4 * r * yd1) / v - wa1);
        frequency_t wa = tanh(ya / v);
        frequency_t yb = yb1 + v * g * (wa - wb1);
        frequency_t wb = tanh(yb / v);
        frequency_t yc = yc1 + v * g * (wb - wc1);
        frequency_t wc = tanh(yc / v);
        frequency_t yd = yd1 + v * g * (wc - tanh(yd1 / v));

        return moog_t{ya,wa,yb,wb,yc,wc,yd};
    }

    template<typename sample_t>
    struct bp_t {
        sample_t x, x1, x2, c1, c2, gain;
    };

    template<typename sample_t>
    sample_t sigbp_qcos(sample_t f)
    {   
        sample_t HALF_PI = M_PI/2.0;
        if (f >= -(HALF_PI) && f <= HALF_PI)
        {
            sample_t g = f*f;
            return (((g*g*g * (-1.0d/720.0d) + g*g*(1.0d/24.0d)) - g*0.5d) + 1);
        }
        else return (0);
    }

    template<typename sample_t, typename frequency_t> 
    const bp_t<sample_t> bp(
        bp_t<sample_t> state, 
        frequency_t f, 
        sample_t q,
        frequency_t sampleRate
    )
    {
        frequency_t r, oneminusr, omega;
        if (f < 0.001) f = 10;
        if (q < 0) q = 0;
        // x->x_freq = f;
        // x->x_q = q;
        omega = f * (M_2_PI) / sampleRate;
        if (q < 0.001) oneminusr = 1.0f;
        else oneminusr = omega/q;
        if (oneminusr > 1.0f) oneminusr = 1.0f;
        r = 1.0f - oneminusr;
        sample_t c_coef1 = 2.0f * sigbp_qcos(omega) * r;
        // sample_t c_coef1 = 2.0f * cos(omega) * r;
        sample_t c_coef2 = - r * r;
        sample_t c_gain = 2 * oneminusr * (oneminusr + r * omega);

        return bp_t<sample_t>{
            state.x,
            state.x1,
            state.x2,
            c_coef1,
            c_coef2,
            c_gain
        };
    }


    template<typename sample_t> 
    const bp_t<sample_t> update_bp(
        const bp_t<sample_t>& state, 
        const sample_t& in
    )
    {

        sample_t last = state.x1;
        sample_t prev = state.x2;
        sample_t coef1 = state.c1;
        sample_t coef2 = state.c2;
        sample_t gain = state.gain;

        sample_t output =  in + coef1 * last + coef2 * prev;
        sample_t x = gain * output;
        prev = last;
        last = output;
        return bp_t<sample_t>{
            x,
            last,
            prev,
            coef1,
            coef2,
            gain
        };
    }


    

}

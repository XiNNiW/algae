#pragma once
#include <math.h>
#include "math_functions.h"
#include <iostream>

// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::filter{
// http://musicdsp.org/files/Audio-EQ-Cookbook.txt

    using algae::dsp::core::math::clamp;
        
    const auto HALF_PI = M_PI_2; 

    template<typename sample_t, typename frequency_t>
    struct biquad_t {
        frequency_t b0, b1, b2, a1, a2, y1, y2;
        sample_t x1, x2;
    };

    template<typename sample_t, typename frequency_t>
    const inline biquad_t<sample_t,frequency_t> lowpass(
        const biquad_t<sample_t,frequency_t>& state,
        const frequency_t& cutoff,
        const sample_t& q,
        const frequency_t& sampleRate=44100.0 
    ){
        frequency_t _q = q<=0?0.01:q;
        frequency_t w0 = cutoff * 2* M_PI / sampleRate;
        frequency_t a = fabs(sin(w0) / (2 * _q));
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
        const sample_t& quality,
        const frequency_t sampleRate
    ){
        sample_t q = quality>0?quality:0.01;
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
    const inline biquad_t<sample_t,frequency_t> update_biquad(const biquad_t<sample_t,frequency_t>& state, const sample_t& x0){

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
    struct bandpass_t{
        sample_t G, R, b1, b2;
        sample_t x1,x2,y1,y2;
    };

    template<typename sample_t>
    bandpass_t<sample_t> process(bandpass_t<sample_t> state, sample_t input){
        sample_t x = input;
        sample_t x1 = state.x1;
        sample_t x2 = state.x2;
        sample_t y1 = state.y1;
        sample_t y2 = state.y2;
        sample_t G = state.G;
        sample_t R = state.R;
        sample_t b1 = state.b1;
        sample_t b2 = state.b2;
        sample_t y = G*(x - R*x2) + b1*y1 + b2*y2;
        return bandpass_t<sample_t>{G,R,b1,b2,x,x1,y,y1};
    }

    template<typename sample_t, typename frequency_t>
    bandpass_t<sample_t> update_coefficients(bandpass_t<sample_t> state, frequency_t center, sample_t q, sample_t sampleRate=44100){
        sample_t x1 = state.x1;
        sample_t x2 = state.x2;
        sample_t y1 = state.y1;
        sample_t y2 = state.y2;

        center = center>0?center:10.0;
        q = q>0?q:0.1;
        sample_t w = center/sampleRate;
        
        sample_t R = exp(-M_PI*(w/q));

        sample_t G  = 1.0-R;
        sample_t b1 = 2*R*cos(M_2_PI*w);
        sample_t b2 = -R*R;
        return bandpass_t<sample_t>{G,R,b1,b2,x1,x2,y1,y2};
    }

    template<typename sample_t>
    struct vcf_t{
        sample_t re, im;
    };

    template<typename sample_t, typename frequency_t>
    const vcf_t<sample_t> process(
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
    const inline onepole_t<sample_t,frequency_t> update_onepole_hip(
        onepole_t<sample_t,frequency_t> state,
        const sample_t& x, const sample_t& cutoff,
        const frequency_t& sampleRate=44100.0
    ){
        const frequency_t w = M_PI_2 * clamp<sample_t,frequency_t>(fabs(cutoff / sampleRate), 0, (sample_t)0.5);
        const frequency_t a = (1 - sin(w)) / cos(w);
        const frequency_t b = (1 + a) / 2;
        const frequency_t y = b * (x - state.x) + a * state.y;
        state.x = x;
        state.y = y;

        return state;
    }

    template<typename sample_t, typename frequency_t>
    struct moog_t {
        sample_t ya1;
        sample_t wa1;
        sample_t yb1;
        sample_t wb1;
        sample_t yc1;
        sample_t wc1;
        sample_t yd1;
        sample_t r;
        sample_t g;
    };

    template<typename sample_t, typename frequency_t>
    const inline moog_t<sample_t, frequency_t> setFilterParameters(
        moog_t<sample_t, frequency_t> state,
        const frequency_t& frequency,
        const sample_t& q,
        const frequency_t& sampleRate=41000.0
    ){
        sample_t f = frequency;
        state.r = q;
        state.g = 1 - exp(-M_PI_2 * f / sampleRate);
        return state;
    }


    using algae::dsp::core::math::tanh_approx_pade;

    template<typename sample_t, typename frequency_t>
    const inline moog_t<sample_t,frequency_t> update_moog(
        const moog_t<sample_t,frequency_t>& state,
        const sample_t& input,
        const frequency_t& sampleRate=44100.0
    ){

        sample_t ya1 = state.ya1;
        sample_t wa1 = state.wa1;
        sample_t yb1 = state.yb1;
        sample_t wb1 = state.wb1;
        sample_t yc1 = state.yc1;
        sample_t wc1 = state.wc1;
        sample_t yd1 = state.yd1;
        sample_t v = 2;
        sample_t x = input;
        sample_t r = state.r;
        sample_t g = state.g;
        sample_t ya = ya1 + v * g * tanh_approx_pade<sample_t>((x - 4 * r * yd1) / v - wa1);
        sample_t wa = tanh_approx_pade<sample_t>(ya / v);
        sample_t yb = yb1 + v * g * (wa - wb1);
        sample_t wb = tanh_approx_pade<sample_t>(yb / v);
        sample_t yc = yc1 + v * g * (wb - wc1);
        sample_t wc = tanh_approx_pade<sample_t>(yc / v);
        sample_t yd = yd1 + v * g * (wc - tanh_approx_pade<sample_t>(yd1 / v));

        return moog_t<sample_t, frequency_t>{ya,wa,yb,wb,yc,wc,yd,r,g};
    }

    template<typename sample_t>
    struct bp_t {
        sample_t x, x1, x2, c1, c2, gain;
    };

    template<typename sample_t>
    sample_t sigbp_qcos(sample_t f)
    {   
        if (f >= -(HALF_PI) && f <= HALF_PI)
        {
            sample_t g = f*f;
            return (((g*g*g * (-1.0/720.0) + g*g*(1.0/24.0)) - g*0.5) + 1);
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

    template<typename sample_t>
    struct reson_bp_t{
        sample_t b0d;
        sample_t b1d;
        sample_t b2d; 
        sample_t a1d;
        sample_t a2d;
        sample_t x1;
        sample_t x2;
        sample_t y1;
        sample_t y2;
    };

    template<typename sample_t, typename frequency_t>
    const inline reson_bp_t<sample_t> update_coefficients(
        reson_bp_t<sample_t> filter, 
        const frequency_t& freq, const sample_t& q, 
        const sample_t& gain, const frequency_t& sampleRate=44100
    ){
        sample_t _q = q>0?q:0.01;
        sample_t w = 2.0*M_PI*freq;
        sample_t a1 = 1.0/_q;
        sample_t a0 = 1.0;
        sample_t b2 = 0.0;
        sample_t b1 = gain;
        sample_t b0 = 0.0;
        sample_t c = 1.0/tan(0.5*w/sampleRate);
        sample_t csq = c*c;
        sample_t d = a0 + a1*c + csq;

        filter.b0d = (b0 + b1*c + b2*csq)/d;
        filter.b1d = 2*(b0 - b1*c + b2*csq)/d;
        filter.b2d = (b0 - b1*c + b2*csq)/d;
        filter.a1d =  2*(a0 - csq)/d;
        filter.a2d =(a0 - a1*c + csq)/d;

        return filter;
    }

    template<typename sample_t, typename frequency_t>
    const inline reson_bp_t<sample_t> reson_bp(
        const frequency_t& freq, 
        const sample_t& q, const sample_t& gain,
        const frequency_t& sampleRate
    ){
        reson_bp_t<sample_t> filter;

        return update_coefficients(filter, freq, q, gain, sampleRate); 
    }

    template<typename sample_t, typename frequency_t>
    const inline reson_bp_t<sample_t> process(reson_bp_t<sample_t> r, const sample_t& input){
        sample_t xn = input;
        sample_t yn = r.b0d*xn + r.b1d*r.x1 + r.b2d*r.x2 - r.a1d*r.y1 - r.a2d*r.y2;

        r.y2 = r.y1;
        r.y1 = yn;
        r.x2 = r.x1;
        r.x1 = xn;

        return r;
    }
    

}

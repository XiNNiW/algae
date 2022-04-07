#pragma once
#include "filter.h"
#include <math.h>
namespace algae::dsp::core::filter{

    template<typename sample_t>
    struct integrator_t{
        sample_t y1;
    };

    template<typename sample_t>
    integrator_t<sample_t> process_integrator(const integrator_t<sample_t> i, const sample_t input){
        sample_t xn = input;
        sample_t yn = xn + i.y1;
        return integrator_t<sample_t>{yn};
    }

    template<typename sample_t>
    struct leaky_integrator_t{
        sample_t x1;
        sample_t y1;
        sample_t y2;
        sample_t a1;
        sample_t b1;
        sample_t b2;
    };

    template<typename sample_t, typename frequency_t>
    leaky_integrator_t<sample_t> leaky_integrator(
        const leaky_integrator_t<sample_t> i, 
        const frequency_t cutoff, 
        const frequency_t sampleRate
    ){
        // const frequency_t TWO_PI = M_PI+M_PI;
        // frequency_t gamma = exp(TWO_PI*(cutoff/sampleRate));
        // return leaky_integrator_t<sample_t>{
        //     i.x1,
        //     i.y1,
        //     i.y2,
        //     (gamma+1)*M_PI_2,
        //     2*gamma,
        //     gamma*gamma
        // };

        const frequency_t TWO_PI = M_PI+M_PI;
        frequency_t epsilon = 0.01;
        frequency_t R = 0.995;
        frequency_t a = 1.0/(sampleRate + epsilon);
        frequency_t b1 = R + (1.0/(1.0+epsilon/sampleRate));
        frequency_t b2 = (R/(1.0+epsilon/sampleRate));
        return leaky_integrator_t<sample_t>{
            i.x1,
            i.y1,
            i.y2,
            a,
            b1,
            b2
        };
    }

    // this idea is from https://www.researchgate.net/publication/319876490_LP-BLIT_Bandlimited_Impulse_Train_Synthesis_of_Lowpass-filtered_Waveforms
    template<typename sample_t>
    leaky_integrator_t<sample_t> process_leaky_integrator(
        const leaky_integrator_t<sample_t> i, 
        const sample_t input
    )
    {
        sample_t xn = input;
        sample_t a  = i.a1;
        sample_t b1 = i.b1;
        sample_t b2 = i.b2;
        sample_t yn = a*xn - a*i.x1 + b1*i.y1 - b2*i.y2;
        return leaky_integrator_t<sample_t>{
            xn,
            yn,
            i.y1,
            a,
            b1,
            b2
        };
    }    


    template<typename sample_t>
    struct leaky_integrator_onepole_t{
        sample_t x1;
        sample_t y1=-0.5;
        sample_t a1;
        sample_t b1;
    };

    template<typename sample_t, typename frequency_t>
    leaky_integrator_onepole_t<sample_t> leaky_integrator_onepole(
        const leaky_integrator_onepole_t<sample_t> i 
        // const frequency_t cutoff, 
        // const frequency_t sampleRate
    ){
        frequency_t c = 0.9992;
        frequency_t a = M_PI_2+M_PI_2*c;
        return leaky_integrator_onepole_t<sample_t>{
            i.x1,
            i.y1,
            a,
            c
        };
    }

    template<typename sample_t>
    leaky_integrator_onepole_t<sample_t> process_leaky_integrator_onepole(
        const leaky_integrator_onepole_t<sample_t> i,
        const sample_t input
    ){
        sample_t xn = input;
        sample_t a  = i.a1;
        sample_t b1 = i.b1;
        sample_t yn = a*xn - a*i.x1 + b1*i.y1;
        return leaky_integrator_onepole_t<sample_t>{
            xn,
            yn,
            a,
            b1
        };
    }


}
namespace algae::dsp::core::experimental{
    using algae::dsp::core::filter::biquad_t;
    template<typename sample_t, typename frequency_t>
    const inline biquad_t<sample_t> notch(
        biquad_t<sample_t> state,
        const sample_t& cutoff,
        const sample_t& q,
        const frequency_t& sampleRate
    ){
        frequency_t w0 = cutoff * TWO_PI / sampleRate;
        frequency_t a = fabs(sin(w0) / (2 * q));
        frequency_t c = cos(w0);
        frequency_t b0 = 1;
        frequency_t b1 = -2 * c;
        frequency_t b2 = 1;
        frequency_t a0 = 1 + a;
        frequency_t a1 = -2 * c;
        frequency_t a2 = 1 - a;
        state.b0 = b0 / a0;
        state.b1 = b1 / a0;
        state.b2 = b2 / a0;
        state.a1 = a1 / a0;
        state.a2 = a2 / a0;

    //   80 static inline BIQUAD *notch(BIQUAD *bq, sample hz, sample q) {
    //   81   double w0 = hz * twopi / SR;
    //   82   double a = fabs(sin(w0) / (2 * q));
    //   83   double c = cos(w0);
    //   84   double b0 = 1, b1 = -2 * c, b2 = 1;
    //   85   double a0 = 1 + a, a1 = -2 * c, a2 = 1 - a;
    //   86   bq->b0 = b0 / a0;
    //   87   bq->b1 = b1 / a0;
    //   88   bq->b2 = b2 / a0;
    //   89   bq->a1 = a1 / a0;
    //   90   bq->a2 = a2 / a0;
    //   91   return bq;
    //   92 }
        // return biquad_t<sample_t,frequency_t>{b0,b1,b2,a1,a2,state.y1,state.y2,state.x1,state.x2};
        return state;
    }
       using algae::dsp::core::math::tanh_approx_pade;

    using algae::dsp::core::filter::moog_t;
    
    template<typename sample_t, typename frequency_t, size_t BLOCK_SIZE>
    const inline std::pair<moog_t<sample_t,frequency_t>, AudioBlock<sample_t,BLOCK_SIZE>> process(
        moog_t<sample_t,frequency_t> state,
        const AudioBlock<sample_t, BLOCK_SIZE>& input,
        const frequency_t& sampleRate=44100.0
    ){

//  167 static inline sample moog(MOOG *m, sample audio, sample frequency, sample resonance) {
//  168   MOOG *muug = m;
//  169   double ya1 = muug->ya1;
//  170   double wa1 = muug->wa1;
//  171   double yb1 = muug->yb1;
//  172   double wb1 = muug->wb1;
//  173   double yc1 = muug->yc1;
//  174   double wc1 = muug->wc1;
//  175   double yd1 = muug->yd1;
//  176   double v = 2;
//  177   double x = audio;
//  178   double f = frequency;
//  179   double r = resonance;
//  180   double g = 1 - exp(-twopi * f / SR);
//  181   double ya = ya1 + v * g * tanh((x - 4 * r * yd1) / v - wa1);
//  182   double wa = tanh(ya / v);
//  183   double yb = yb1 + v * g * (wa - wb1);
//  184   double wb = tanh(yb / v);
//  185   double yc = yc1 + v * g * (wb - wc1);
//  186   double wc = tanh(yc / v);
//  187   double yd = yd1 + v * g * (wc - tanh(yd1 / v));
//  188   double y = yd;
//  189   muug->ya1 = ya;
//  190   muug->wa1 = wa;
//  191   muug->yb1 = yb;
//  192   muug->wb1 = wb;
//  193   muug->yc1 = yc;
//  194   muug->wc1 = wc;
//  195   muug->yd1 = yd;
//  196   return y;
//  197 }
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
        const sample_t r = state.r;
        const sample_t g = state.g;
        const sample_t v = 2;

        AudioBlock<sample_t,BLOCK_SIZE> output;

        for(size_t idx=0; idx < BLOCK_SIZE; idx++){
            sample_t ya1 = state.ya1;
            sample_t wa1 = state.wa1;
            sample_t yb1 = state.yb1;
            sample_t wb1 = state.wb1;
            sample_t yc1 = state.yc1;
            sample_t wc1 = state.wc1;
            sample_t yd1 = state.yd1;
            sample_t x = input[idx];

            sample_t ya = ya1 + v * g * tanh_approx_pade<sample_t>((x - 4 * r * yd1) / v - wa1);
            state.ya1 = ya;
            sample_t wa = tanh_approx_pade<sample_t>(ya / v);
            state.wa1 = wa;
            sample_t yb = yb1 + v * g * (wa - wb1);
            state.yb1 = yb;
            sample_t wb = tanh_approx_pade<sample_t>(yb / v);
            state.wb1 = wb;
            sample_t yc = yc1 + v * g * (wb - wc1);
            state.yc1 = yc;
            sample_t wc = tanh_approx_pade<sample_t>(yc / v);
            state.wc1 = yc;
            sample_t yd = yd1 + v * g * (wc - tanh_approx_pade<sample_t>(yd1 / v));
            state.yd1 = yd;
                                
            output[idx] = ya;
        } 
        return std::pair(state,output);
    }

      template<typename sample_t>
    struct bp_t {
        sample_t x, x1, x2, c1, c2, gain;
    };

    template<typename sample_t>
    const inline sample_t sigbp_qcos(sample_t f)
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
        const bp_t<sample_t>& state, 
        const frequency_t& f, 
        const sample_t& q,
        const frequency_t& sampleRate
    )
    {
        frequency_t r, oneminusr, omega;
        if (f < 0.001) f = 10;
        if (q < 0) q = 0;
        // x->x_freq = f;
        // x->x_q = q;
        omega = f * (TWO_PI) / sampleRate;
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


    template<typename sample_t, size_t BLOCK_SIZE> 
    const inline bp_t<sample_t> process(
        const bp_t<sample_t>& state, 
        const AudioBlock<sample_t, BLOCK_SIZE>& in
    )
    {
        const sample_t coef1 = state.c1;
        const sample_t coef2 = state.c2;
        const sample_t gain = state.gain;

        AudioBlock<sample_t, BLOCK_SIZE> output;

        for(size_t idx=0; idx< BLOCK_SIZE; idx++){
            sample_t last = state.x1;
            sample_t prev = state.x2;
            

            sample_t output =  in[idx] + coef1 * last + coef2 * prev;
            state.x = gain * output;
            state.x1 = last;
            state.x2 = output;
            output[idx] = output;
        }
        
        return std::pair(state,output);
    }

     template<typename sample_t>
    struct bandpass_t{
        sample_t G, R, b1, b2;
        sample_t x1,x2,y1,y2;
    };

    template<typename sample_t, size_t BLOCK_SIZE>
    const inline std::pair<bandpass_t<sample_t>,AudioBlock<sample_t,BLOCK_SIZE>> process(bandpass_t<sample_t> state, const AudioBlock<sample_t, BLOCK_SIZE> input){
        
        sample_t G = state.G;
        sample_t R = state.R;
        sample_t b1 = state.b1;
        sample_t b2 = state.b2;
        AudioBlock<sample_t,BLOCK_SIZE> output;
        for(size_t idx=0;idx<BLOCK_SIZE;idx++){
            sample_t x0 = input[idx];
            sample_t x1 = state.x1;
            sample_t x2 = state.x2;
            sample_t y1 = state.y1;
            sample_t y2 = state.y2;
            state.y1 = G*(x0 - R*x2) + b1*y1 + b2*y2;
            state.y2 = y1;
            state.x1 = x0;
            state.x2 = x1;
            output[idx] = state.y1;
        }
        return std::pair(state,output);
    }

    template<typename sample_t, typename frequency_t>
    const inline bandpass_t<sample_t> update_coefficients(bandpass_t<sample_t> state, const frequency_t& center, const sample_t& q, const sample_t& sampleRate=44100){
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

        state.R=R;
        state.G=G;
        state.b1 = b1;
        state.b2 = b2;
        return state;
    }

    template<typename sample_t>
    struct vcf_t{
        sample_t re, im;
    };

    template<typename sample_t, typename frequency_t, size_t BLOCK_SIZE>
    const std::pair<vcf_t<sample_t>, AudioBlock<sample_t,BLOCK_SIZE>> process(
        vcf_t<sample_t> state, 
        const AudioBlock<sample_t,BLOCK_SIZE>& x, 
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

        AudioBlock<sample_t,BLOCK_SIZE> output;
        for(size_t idx=0;idx<BLOCK_SIZE;idx++){
            sample_t re2 = state.re;
            state.re = ampcorrect * oneminusr * x[idx] + cre * re2 - cim * state.im;
            state.im = cim * re2 + cre * state.im;
            output[idx] = state.re;
        }
        
        return std::pair(state,output);
    }


    //Moore, Elements of Computer Music, 1990
    template<typename frequency_t>
    int max_number_of_harmonics(frequency_t freq, frequency_t sampleRate){
        int number_of_harmonics_before_aliasing = floor(sampleRate/(2.0*freq));
        return number_of_harmonics_before_aliasing;
    }

    // Stilson & Smith, Alias-Free Digital Synthesis of Classic Analog Waveforms, https://ccrma.stanford.edu/~stilti/papers/blit.pdf
    template<typename sample_t>
    sample_t sincM(int m_number_of_harmonics, sample_t theta){
        sample_t m = m_number_of_harmonics;
        return sin(M_PI*theta)/(m*sin(m*M_PI*theta));
    }

    // Stilson & Smith, Alias-Free Digital Synthesis of Classic Analog Waveforms, https://ccrma.stanford.edu/~stilti/papers/blit.pdf
    template<typename sample_t>
    sample_t blit(int m_number_of_harmonics, int period, sample_t theta){
        sample_t m = m_number_of_harmonics;
        sample_t p = period;
        sample_t m_p = (m/p);
        
        return (m_p)*sincM(m,m_p*theta);
    }

    //Moore, Elements of Computer Music, 1990
    template<typename sample_t>
    sample_t Bn(int n, sample_t theta){
        
        auto isOdd = [](int n){return (n%2)!=0;};
        sample_t theta_pi = theta/M_PI;
        int m = floor(theta_pi);

        return (theta_pi-double(m)) > 0 
                    ? (sin(sample_t(n)*theta*.5)/sin(theta*.5))
                    : isOdd(n)
                    ? n 
                    : isOdd(m)
                    ? -n
                    : n;
    }

    template<typename sample_t>
    const sample_t dsf_bl_pulse(
        const sample_t theta_phase,  
        const sample_t beta_phase, 
        const int number_of_harmonics
    ){
        sample_t theta = TWO_PI*theta_phase;
        sample_t beta = TWO_PI*beta_phase;
        int n = number_of_harmonics;

        sample_t B = Bn(n, beta);
                    
        return (1.0/sample_t(n))*sin(theta + sample_t(n-1)*beta*0.5)*B;
    }

    template<typename sample_t>
    const sample_t fm_bl_pulse(
        const sample_t phase, 
        const sample_t modulation_index
    ){
        sample_t wt = phase*TWO_PI;
        sample_t k = modulation_index;
        return exp(k*cos(wt)-k)*cos(wt);
    }

    template<typename sample_t>
    const sample_t fm_bl_bipolar_pulse(
        const sample_t phase, 
        const sample_t modulation_index
    ){
        sample_t wt = phase*TWO_PI;
        sample_t k = modulation_index;
        return exp(k*cos(2*wt)-k)*cos(wt);
    }

    template<typename frequency_t>
    frequency_t max_bl_modulation_index(
        const frequency_t& carrierFrequency,
        const frequency_t& modulatorFrequency,
        const frequency_t& sampleRate
    ){
        const frequency_t& fc = carrierFrequency;
        const frequency_t& fm = modulatorFrequency;
        return fm > 0 ? (sampleRate/3.0 - fc) / fm : 0;
    }

    using algae::dsp::core::filter::dc_block_t;
    using algae::dsp::core::filter::process;

    using algae::dsp::core::filter::leaky_integrator_t;
    using algae::dsp::core::filter::leaky_integrator;
    using algae::dsp::core::filter::process_leaky_integrator;

    using algae::dsp::core::filter::leaky_integrator_onepole_t;
    using algae::dsp::core::filter::leaky_integrator_onepole;
    using algae::dsp::core::filter::process_leaky_integrator_onepole;

    template<typename sample_t, typename frequency_t>
    struct fm_bl_saw_t{
        frequency_t freq;
        sample_t index;
        phasor_t<sample_t> phasor;
        leaky_integrator_t<sample_t> integrator;
        sample_t output;
    };

    template<typename sample_t, typename frequency_t>
    const inline fm_bl_saw_t<sample_t,frequency_t> setFrequency(
        fm_bl_saw_t<sample_t,frequency_t> state,
        const frequency_t& freq,
        const sample_t& sampleRate = 41000
    ){
        
        state.freq = freq;
        state.index = max_bl_modulation_index(freq,freq,sampleRate);
        return state;
        
    }

    template<typename sample_t, typename frequency_t>
    const fm_bl_saw_t<sample_t,frequency_t> fm_bl_saw(
        const fm_bl_saw_t<sample_t,frequency_t> state,
        const frequency_t freq,
        const sample_t sampleRate
    ){
        
        return fm_bl_saw_t<sample_t,frequency_t> {
            freq,
            max_bl_modulation_index(freq, freq, sampleRate),
            phasor_t<frequency_t>(),
            leaky_integrator<sample_t, frequency_t>(state.integrator, 19.0, sampleRate),
            0
        };
        
    }

    template<typename sample_t, typename frequency_t>
    const fm_bl_saw_t<sample_t,frequency_t> process_fm_bl_saw(
        const fm_bl_saw_t<sample_t,frequency_t> state,
        const sample_t sampleRate
    ){
        fm_bl_saw_t<sample_t,frequency_t> saw = state;

        sample_t pulse = fm_bl_pulse(saw.phasor.phase, saw.index);
        saw.integrator = process_leaky_integrator<sample_t>(saw.integrator, pulse);
        saw.phasor = update_phasor<sample_t,frequency_t>(saw.phasor, saw.freq, sampleRate);
        saw.output = saw.integrator.y1;
        return saw;
    }   

    template<typename sample_t, typename frequency_t>
    struct fm_bl_square_t{
        frequency_t freq;
        sample_t index;
        phasor_t<sample_t> phasor;
        leaky_integrator_t<sample_t> integrator;
        sample_t output;
    };

    template<typename sample_t, typename frequency_t>
    const fm_bl_square_t<sample_t,frequency_t> setFrequency(
        fm_bl_square_t<sample_t,frequency_t> state,
        const frequency_t freq,
        const sample_t sampleRate
    ){
        
        state.freq = freq;
        state.index = max_bl_modulation_index(freq,2*freq,sampleRate);
        return state;
        
    }

    template<typename sample_t, typename frequency_t>
    const fm_bl_square_t<sample_t,frequency_t> fm_bl_square(
        const fm_bl_square_t<sample_t,frequency_t> state,
        const frequency_t freq,
        const sample_t sampleRate
    ){
        
        return fm_bl_square_t<sample_t,frequency_t>{
            freq,
            max_bl_modulation_index(freq, freq, sampleRate),
            phasor_t<frequency_t>(),
            leaky_integrator<sample_t, frequency_t>(state.integrator, 19.0, sampleRate),
            0
        };
        
    }

    template<typename sample_t, typename frequency_t>
    const fm_bl_square_t<sample_t,frequency_t> process_fm_bl_square(
        const fm_bl_square_t<sample_t,frequency_t> state,
        const sample_t sampleRate
    ){
        fm_bl_square_t<sample_t,frequency_t> square = state;

        sample_t pulse = fm_bl_bipolar_pulse(square.phasor.phase,square.index);
        square.integrator = process_leaky_integrator<sample_t>(square.integrator, pulse);
        square.phasor = update_phasor<sample_t,frequency_t>(square.phasor, square.freq, sampleRate);
        square.output = square.integrator.y1;
        return square;
    }

    template<typename sample_t, typename frequency_t>
    struct fm_bl_tri_t{
        frequency_t freq;
        sample_t index;
        phasor_t<sample_t> phasor;
        leaky_integrator_t<sample_t> integrator1;
        leaky_integrator_t<sample_t> integrator2;
        sample_t output;
    };

    template<typename sample_t, typename frequency_t>
    const fm_bl_tri_t<sample_t,frequency_t> fm_bl_tri(
        const fm_bl_tri_t<sample_t,frequency_t> state,
        const frequency_t freq,
        const sample_t sampleRate
    ){
        
        return fm_bl_tri_t<sample_t,frequency_t>{
            freq,
            max_bl_modulation_index(freq, freq, sampleRate),
            phasor_t<frequency_t>(),
            leaky_integrator<sample_t, frequency_t>(state.integrator1,20.0,sampleRate),
            leaky_integrator<sample_t, frequency_t>(state.integrator2,20.0,sampleRate),
            0
        };
        
    }

    template<typename sample_t, typename frequency_t>
    const fm_bl_tri_t<sample_t,frequency_t> setFrequency(
        fm_bl_tri_t<sample_t,frequency_t> state,
        const frequency_t freq,
        const sample_t sampleRate
    ){
        
        state.freq = freq;
        state.index = max_bl_modulation_index(freq,2*freq,sampleRate);
        return state;
        
    }

    template<typename sample_t, typename frequency_t>
    const fm_bl_tri_t<sample_t,frequency_t> process_fm_bl_tri(
        const fm_bl_tri_t<sample_t,frequency_t> state,
        const sample_t sampleRate
    ){
        fm_bl_tri_t<sample_t,frequency_t> tri = state;

        sample_t pulse = fm_bl_bipolar_pulse<sample_t>(tri.phasor.phase,tri.index);
        tri.integrator1 = process_leaky_integrator<sample_t>(tri.integrator1, pulse);
        tri.integrator2 = process_leaky_integrator<sample_t>(tri.integrator2, tri.integrator1.y1);
        tri.phasor = update_phasor<sample_t,frequency_t>(tri.phasor, tri.freq, sampleRate);
        tri.output = tri.integrator2.y1;
        return tri;
    }


    //LP-BLIT: BANDLIMITED IMPULSE TRAIN SYNTHESIS OF LOWPASS-FILTEREDWAVEFORMSSebastian Kraft, Udo Zölzer
    template<typename sample_t>
    struct lp_integrator_t{
        sample_t x1;
        sample_t y1;
        sample_t y2;
        sample_t a;
        sample_t b1;
        sample_t b2;
    };

    template<typename sample_t, typename frequency_t> 
    lp_integrator_t<sample_t> lp_integrator( frequency_t hipass_cutoff, frequency_t sampleRate){
        
        sample_t x1=0;
        sample_t y1=0;
        sample_t y2=0;
        sample_t gamma = 0.9992;//exp(-TWO_PI*hipass_cutoff/sampleRate);//
        sample_t a = M_PI*(gamma+1.0)/2.0;
        sample_t b1 = 2.0*gamma;
        sample_t b2 = gamma*gamma;

        return lp_integrator_t<sample_t>{
            x1,
            y1,
            y2,
            a,
            b1,
            b2
        };
    }

    template<typename sample_t> 
    lp_integrator_t<sample_t> process(lp_integrator_t<sample_t> integrator, sample_t xn){
        sample_t yn = integrator.a*(xn - integrator.x1) + integrator.b1*integrator.y1 - integrator.b2*integrator.y2;
        return lp_integrator_t<sample_t>{
            xn,
            yn,
            integrator.y1,
            integrator.a,
            integrator.b1,
            integrator.b2
        };
    }




    
    template<typename sample_t, typename frequency_t>
    struct lp_blit_square_t{
        sample_t phase;
        frequency_t frequency;
        sample_t alpha;
        int num_harmonics;
        lp_integrator_t<sample_t> integrator;
    };

    template<typename sample_t, typename frequency_t>
    lp_blit_square_t<sample_t, frequency_t> lp_blit_square(frequency_t frequency, frequency_t hipass_cutoff, frequency_t sampleRate){
        sample_t phase = 0;
        sample_t alpha = 1;
        int num_harmonics = max_number_of_harmonics(frequency, sampleRate)-1;

        return lp_blit_square_t<sample_t, frequency_t>{
            phase,
            frequency,
            alpha,
            num_harmonics,
            lp_integrator<sample_t, frequency_t>(hipass_cutoff, sampleRate)
        };
    }

    template<typename sample_t, typename frequency_t>
    lp_blit_square_t<sample_t, frequency_t> setFrequency(lp_blit_square_t<sample_t, frequency_t> blit, frequency_t frequency, frequency_t sampleRate){
        int num_harmonics = max_number_of_harmonics(frequency, sampleRate)-1;
       
        return lp_blit_square_t<sample_t, frequency_t>{
            blit.phase,
            frequency,
            blit.alpha,
            num_harmonics,
            blit.integrator
        };
    }

    template<typename sample_t, typename frequency_t>
    lp_blit_square_t<sample_t, frequency_t> setBrightness(lp_blit_square_t<sample_t, frequency_t> blit, sample_t alpha){

        return lp_blit_square_t<sample_t, frequency_t>{
            blit.phase,
            blit.frequency,
            alpha,
            blit.num_harmonics,
            blit.integrator
        };
    }



    template<typename sample_t, typename frequency_t>
    lp_blit_square_t<sample_t, frequency_t> process(lp_blit_square_t<sample_t, frequency_t> blit, frequency_t sampleRate){
        
        sample_t omega = sample_t(blit.num_harmonics)*TWO_PI*blit.frequency*(TWO_PI*blit.phase-M_PI)/sampleRate;
        sample_t xn = sin(M_PI*blit.phase)*blit.alpha*sin(omega)/sinh(blit.alpha*omega);
        // sample_t xi = sin(M_PI*blit.phase)*blit.alpha*sin(2*omega)/sinh(2*blit.alpha*omega);
        sample_t phase = update_phase<sample_t, frequency_t>(blit.phase, blit.frequency, sampleRate);
        // xn = xn - xi;
        // lp_integrator_t<sample_t> i = process(blit.integrator,xn);

        return lp_blit_square_t<sample_t, frequency_t>{
            phase,
            blit.frequency,
            blit.alpha,
            blit.num_harmonics,
            process(blit.integrator,xn)
        };
    }
}
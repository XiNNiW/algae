#pragma once
#include <math.h>
#include "dc_blocker.h"
#include "integrator.h"


// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::oscillator{

    const auto TWO_PI = 2.0*M_PI;

    template<typename sample_t>
    const sample_t sinOsc(const sample_t& phase){
        return sin(TWO_PI*phase);
    }

    template<typename sample_t>
    const sample_t cosOsc(const sample_t& phase){
        return cos(TWO_PI*phase);
    }



    template<typename sample_t>
    const sample_t noise(){
        return (sample_t) 2.0 * (((sample_t) rand() / (sample_t) RAND_MAX) - (sample_t)0.5);
    }

    template<typename frequency_t>
    struct phasor_t{
        frequency_t phase=0;
        // frequency_t freq=0;
        // phasor_t()=default;
        // phasor_t(frequencDy_t phase):phase(phase){}
    };

    template<typename sample_t, typename frequency_t>
    const sample_t osc(const phasor_t<frequency_t>& phasor){
        return cosf128(TWO_PI*phasor.phase);
    }

    template<typename sample_t, typename frequency_t>
    const phasor_t<frequency_t> update_phasor(phasor_t<frequency_t> phasor_state, const frequency_t& freq, const frequency_t& sampleRate=44100.0){
        frequency_t phi = freq/sampleRate;
        frequency_t phase = phasor_state.phase;
        phase += phi;
        phase -= floor(phase);
        return phasor_t<frequency_t>{ phase };
    }

    template<typename sample_t, typename frequency_t>
    sample_t update_phase(sample_t phase, frequency_t freq, frequency_t sampleRate){
        frequency_t phi = freq/sampleRate;
        phase += phi;
        phase -= floor(phase);
        return phase;
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
    using algae::dsp::core::filter::process_dc_blocker;

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
    const fm_bl_saw_t<sample_t,frequency_t> setFrequency(
        fm_bl_saw_t<sample_t,frequency_t> state,
        const frequency_t freq,
        const sample_t sampleRate
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

    



    // template<typename sample_t, typename frequency_t>
    // struct lp_blit_saw_t{
    //     sample_t phase;
    //     frequency_t frequency;
    //     sample_t alpha;
    //     int num_harmonics;
    //     lp_integrator_t<sample_t> integrator1;
    //     lp_integrator_t<sample_t> integrator2;
    // };

    // template<typename sample_t, typename frequency_t>
    // lp_blit_saw_t<sample_t, frequency_t> lp_blit_saw(frequency_t frequency, frequency_t hipass_cutoff, frequency_t sampleRate){
    //     sample_t phase = 0;
    //     sample_t alpha = 1;
    //     int num_harmonics = max_number_of_harmonics(frequency, sampleRate)-1;

    //     return lp_blit_saw_t<sample_t, frequency_t>{
    //         phase,
    //         frequency,
    //         alpha,
    //         num_harmonics,
    //         lp_integrator<sample_t, frequency_t>(hipass_cutoff, sampleRate),
    //         lp_integrator<sample_t, frequency_t>(hipass_cutoff, sampleRate)
    //     };
    // }

    // template<typename sample_t, typename frequency_t>
    // lp_blit_saw_t<sample_t, frequency_t> setFrequency(lp_blit_saw_t<sample_t, frequency_t> blit, frequency_t frequency, frequency_t sampleRate){
    //     int num_harmonics = max_number_of_harmonics(frequency, sampleRate)-1;
       
    //     return lp_blit_saw_t<sample_t, frequency_t>{
    //         blit.phase,
    //         frequency,
    //         blit.alpha,
    //         num_harmonics,
    //         blit.integrator1,
    //         blit.integrator2
    //     };
    // }

    // template<typename sample_t, typename frequency_t>
    // lp_blit_saw_t<sample_t, frequency_t> setBrightness(lp_blit_square_t<sample_t, frequency_t> blit, sample_t alpha){

    //     return lp_blit_saw_t<sample_t, frequency_t>{
    //         blit.phase,
    //         blit.frequency,
    //         alpha,
    //         blit.num_harmonics,
    //         blit.integrator1,
    //         blit.integrator2
    //     };
    // }



    // template<typename sample_t, typename frequency_t>
    // lp_blit_saw_t<sample_t, frequency_t> process(lp_blit_saw_t<sample_t, frequency_t> blit, frequency_t sampleRate){
        
    //     sample_t omega = TWO_PI*blit.frequency*blit.phase*sample_t(blit.num_harmonics)/sampleRate;
    //     sample_t xn = sin(blit.phase*M_PI)*blit.alpha*sin(omega-M_PI)/sinh(blit.alpha*omega - M_PI);
    //     sample_t phase = update_phase<sample_t, frequency_t>(blit.phase, blit.frequency, sampleRate);
    //     lp_integrator_t<sample_t> integrator1 = process(blit.integrator1,xn);
    //     lp_integrator_t<sample_t> integrator2 = process(blit.integrator2,blit.integrator1.y1);
    //     return lp_blit_saw_t<sample_t, frequency_t>{
    //         phase,
    //         blit.frequency,
    //         blit.alpha,
    //         blit.num_harmonics,
    //         integrator1,
    //         integrator2
    //     };
    // }







}


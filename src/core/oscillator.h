#pragma once
#include <math.h>
#include "dc_blocker.h"
#include "integrator.h"
#include <array>


// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::oscillator{

    const double TWO_PI = 2.0*M_PI;

    template<typename sample_t, unsigned int x>
    struct factorial_t{
        static constexpr sample_t result=x*factorial_t<sample_t, x-1>::result;
    };

    template<typename sample_t>
    struct factorial_t<sample_t,1>{
        static constexpr sample_t result=1;
    };

    template<typename sample_t>
    constexpr const sample_t _sin(const sample_t phase){
        sample_t x = phase;
        sample_t x2 = x*x;
        sample_t x3 = x2*x;
        sample_t x5 = x2*x3;
        sample_t x7 = x2*x5;
        sample_t x9 = x2*x7;
        sample_t x11 = x2*x9;
        sample_t x13 = x2*x11;
        sample_t x15 = x2*x13;
        sample_t x17 = x2*x15;
        sample_t x19 = x2*x17;

        return x
            -(x3/factorial_t<sample_t,3>::result)
            +(x5/factorial_t<sample_t,5>::result)
            -(x7/factorial_t<sample_t,7>::result)
            +(x9/factorial_t<sample_t,9>::result)
            -(x11/factorial_t<sample_t,11>::result)
            +(x13/factorial_t<sample_t,13>::result)
            -(x15/factorial_t<sample_t,15>::result)
            +(x17/factorial_t<sample_t,17>::result)
            -(x19/factorial_t<sample_t,19>::result)
            ;
    }

    template<typename sample_t>
    constexpr const sample_t _cos(const sample_t phase){
        sample_t x = phase;
        sample_t x2 = x*x;
        sample_t x4 = x2*x2;
        sample_t x6 = x2*x4;
        sample_t x8 = x2*x6;
        sample_t x10 = x2*x8;
        sample_t x12 = x2*x10;
        sample_t x14 = x2*x12;
        sample_t x16 = x2*x14;
        sample_t x18 = x2*x16;

        return 1
            -(x2/factorial_t<sample_t,2>::result)
            +(x4/factorial_t<sample_t,4>::result)
            -(x6/factorial_t<sample_t,6>::result)
            +(x8/factorial_t<sample_t,8>::result)
            -(x10/factorial_t<sample_t,10>::result)
            +(x12/factorial_t<sample_t,12>::result)
            -(x14/factorial_t<sample_t,14>::result)
            +(x16/factorial_t<sample_t,16>::result)
            -(x18/factorial_t<sample_t,18>::result)
            ;

    }

    // template<typename sample_t, int SIZE, std::size_t... INDEX>
    // constexpr const std::array<sample_t, SIZE> _makeSineTable_debug(const std::index_sequence<INDEX...> index_seq){
       
    //     const sample_t phase_increment = 2.0*M_PI/sample_t(SIZE);
    //     ((std::cout<<"increment: "<<sample_t(INDEX)*phase_increment<<"\n"),...);
    //     return {{_sin<sample_t>(sample_t(INDEX)*phase_increment)...}};

    // }

    // template<typename sample_t, int SIZE, typename Indices = std::make_index_sequence<SIZE>>
    // constexpr const std::array<sample_t, SIZE> makeSineTable_debug(){
    //     return _makeSineTable_debug<sample_t, SIZE>(Indices{});
    // }

    template<typename sample_t, int SIZE, std::size_t... INDEX>
    constexpr const std::array<sample_t, SIZE> _makeSineTable(const std::index_sequence<INDEX...> index_seq){
       
        const sample_t phase_increment = 2.0*M_PI/sample_t(SIZE);
        // return {{sin(sample_t(INDEX)*phase_increment)...}};
        return {{_sin<sample_t>(sample_t(INDEX)*phase_increment)...}};

    }

    template<typename sample_t, int SIZE, typename Indices = std::make_index_sequence<SIZE>>
    constexpr const std::array<sample_t, SIZE> makeSineTable(){
        return _makeSineTable<sample_t, SIZE>(Indices{});
    }

    template<typename sample_t, int SIZE, std::size_t... INDEX>
    constexpr const std::array<sample_t, SIZE> _makeCosTable(const std::index_sequence<INDEX...> index_seq){
       
        const sample_t phase_increment = 2.0*M_PI/sample_t(SIZE);
        // return {{cos(sample_t(INDEX)*phase_increment)...}};
        return {{_cos<sample_t>(sample_t(INDEX)*phase_increment)...}};

    }

    template<typename sample_t, int SIZE, typename Indices = std::make_index_sequence<SIZE>>
    constexpr const std::array<sample_t, SIZE> makeCosTable(){
        return _makeCosTable<sample_t,SIZE>(Indices{});
    }

    template<typename sample_t, int SIZE>
    sample_t table_lookup_lin_interp(const sample_t* table, const sample_t& phase){
        sample_t _phase = fmod(abs(phase),1);
        // _phase = _phase>1 ? _phase - floor(_phase) : _phase;
        sample_t position = _phase*(SIZE-1);
        int index = floor(position);
        int x0 = index;
        int x1 = index+1;
        x1 = x1>=SIZE?0:x1;
        sample_t mantissa = position - index;
        sample_t value = (1-mantissa)*table[x0] + mantissa*table[x1];
        return value;
    }

    template<typename sample_t, int SIZE>
    struct sine_t{
        static constexpr std::array<sample_t, SIZE> TABLE = makeSineTable<sample_t,SIZE>();
        inline static const sample_t lookup(const sample_t phase){
            return table_lookup_lin_interp<sample_t,SIZE>(TABLE.data(), phase);
        }
    };

    template<typename sample_t, int SIZE>
    struct cos_t{
        static constexpr std::array<sample_t, SIZE> TABLE = makeCosTable<sample_t,SIZE>();
        inline static const sample_t lookup(const sample_t phase){
            return table_lookup_lin_interp<sample_t,SIZE>(TABLE.data(), phase);
        }
    };

    template<typename sample_t>
    sample_t tanh_approx_pade(sample_t x)
    {
        if( x < -3 )
            return -1;
        else if( x > 3 )
            return 1;
        else
            return x * ( 27 + x * x ) / ( 27 + 9 * x * x );
    }

    template<typename sample_t>
    sample_t tanh_approx_pade_noclip(sample_t x)
    {
        return x * ( 27 + x * x ) / ( 27 + 9 * x * x );
    }

    template<typename sample_t>
    sample_t sine_approx_taylor_4(sample_t x)
    {
        sample_t x2 = x*x;
        sample_t x3 = x*x2;
        sample_t x5 = x2*x3;
        sample_t x7 = x2*x5;
        sample_t x9 = x2*x7;
        return x
            -(x3/factorial_t<sample_t,3>::result)
            +(x5/factorial_t<sample_t,5>::result)
            -(x7/factorial_t<sample_t,7>::result)
            +(x9/factorial_t<sample_t,9>::result);
       
    }

    template<typename sample_t>
    sample_t cos_approx_taylor_4(sample_t x)
    {
        sample_t x2 = x*x;
        sample_t x4 = x2*x2;
        sample_t x6 = x2*x4;
        sample_t x8 = x2*x6;
        return x
            -(x2/factorial_t<sample_t,2>::result)
            +(x4/factorial_t<sample_t,4>::result)
            -(x6/factorial_t<sample_t,6>::result)
            +(x8/factorial_t<sample_t,8>::result);
       
    }

    template<typename sample_t, typename frequency_t>
    const sample_t update_phase_custom_period(sample_t phase, frequency_t increment, frequency_t period=TWO_PI){
        phase += period*increment;
        if ( phase >= period ) phase -= period;
        return phase;
    }

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

    template<typename sample_t, typename frequency_t>
    struct stk_blit_saw_t{
        int m;
        sample_t phase_increment;
        sample_t phase;
        sample_t p;
        sample_t c2;
        sample_t a;
        sample_t state;
        sample_t lastFrame;
    };
    template<typename sample_t, typename frequency_t>
    stk_blit_saw_t<sample_t, frequency_t> setFrequency(stk_blit_saw_t<sample_t, frequency_t> saw, frequency_t frequency, frequency_t sampleRate){

        saw.p = sampleRate/frequency;
        saw.c2 = 1/saw.p;
        saw.phase_increment = M_PI*saw.c2;

        int max_harmonics = floor(0.5*saw.p);
        saw.m = 2*max_harmonics+1;
        saw.a = saw.m / saw.p;

        return saw;
    }
    
    template<typename sample_t, typename frequency_t>
    stk_blit_saw_t<sample_t, frequency_t> stk_blit_saw(frequency_t frequency, frequency_t sampleRate){
        stk_blit_saw_t<sample_t, frequency_t> saw;

        saw = setFrequency(saw, frequency, sampleRate);
        saw.state = -0.5 * saw.a;
        return saw;
    }

    template<typename sample_t, typename frequency_t>
    stk_blit_saw_t<sample_t, frequency_t> process(stk_blit_saw_t<sample_t, frequency_t> saw, frequency_t sampleRate){
        sample_t tmp, denominator = sin( saw.phase );
        if ( fabs(denominator) <= std::numeric_limits<sample_t>::epsilon() )
            tmp = saw.a;
        else {
            tmp =  sin( saw.m * saw.phase );
            tmp /= saw.p * denominator;
        }

        tmp += saw.state - saw.c2;
        saw.state = tmp * 0.995;

        saw.phase += saw.phase_increment;
        if ( saw.phase >= M_PI ) saw.phase -= M_PI;
            
        saw.lastFrame = tmp;
        return saw;
    }

    template<typename sample_t, typename frequency_t>
    struct stk_blit_square_t{
        int m;
        sample_t phase_increment;
        sample_t phase;
        sample_t p;
        sample_t c2;
        sample_t a;
        sample_t state;
        sample_t lastFrame;
        sample_t last_blit_output;
        sample_t dc_blocker_x1;
    };
    template<typename sample_t, typename frequency_t>
    stk_blit_square_t<sample_t, frequency_t> setFrequency(stk_blit_square_t<sample_t, frequency_t> square, frequency_t frequency, frequency_t sampleRate){

        square.p = 0.5*sampleRate/frequency;
        // square.c2 = 1/square.p;
        square.phase_increment = M_PI/square.p;

        int max_harmonics = floor(0.5*square.p);
        square.m = 2*(max_harmonics+1);
        square.a = square.m / square.p;

        return square;
    }
    
    template<typename sample_t, typename frequency_t>
    stk_blit_square_t<sample_t, frequency_t> stk_blit_square(frequency_t frequency, frequency_t sampleRate){
        stk_blit_square_t<sample_t, frequency_t> square;

        square = setFrequency(square, frequency, sampleRate);
        // square.state = -0.5 * square.a;
        return square;
    }

    template<typename sample_t, typename frequency_t>
    stk_blit_square_t<sample_t, frequency_t> process(stk_blit_square_t<sample_t, frequency_t> square, frequency_t sampleRate){
        
        sample_t tmp = square.last_blit_output;
        sample_t denominator = sin( square.phase );
        if ( fabs(denominator) <= std::numeric_limits<sample_t>::epsilon() )
            if ( square.phase < 0.1f || square.phase > TWO_PI - 0.1 )
                square.last_blit_output = square.a;
            else
                square.last_blit_output = -square.a;
        else {
            square.last_blit_output =  sin( square.m * square.phase );
            square.last_blit_output /= square.p * denominator;
        }

        square.last_blit_output += tmp;
        square.state = square.last_blit_output - square.dc_blocker_x1 + 0.999*square.state;

        square.dc_blocker_x1 = square.last_blit_output;

        square.phase += square.phase_increment;
        if ( square.phase >= TWO_PI ) square.phase -= TWO_PI;
            
        return square;
    }



    

}


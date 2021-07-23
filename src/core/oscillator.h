#pragma once
#include <math.h>
#include "audio_block.h"
#include "dc_blocker.h"
#include <array>


// Heavily inspired by CLiVE by Claude Heiland-Allen https://code.mathr.co.uk/clive
namespace algae::dsp::core::oscillator{

    const auto TWO_PI = 2.0*M_PI;

    template<typename sample_t, typename frequency_t>
    const inline sample_t update_phase(sample_t phase, const frequency_t& increment, const frequency_t& period=1){
        phase += period*increment;
        if ( phase >= period ) phase -= period;
        return phase;
    }

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

    template<typename sample_t, int TABLE_SIZE, std::size_t... INDEX>
    constexpr const std::array<sample_t, TABLE_SIZE> _makeSineTable(const std::index_sequence<INDEX...> index_seq){
       
        const sample_t phase_increment = 2.0*M_PI/sample_t(TABLE_SIZE);
        return {{_sin<sample_t>(sample_t(INDEX)*phase_increment)...}};

    }

    template<typename sample_t, int TABLE_SIZE, typename Indices = std::make_index_sequence<TABLE_SIZE>>
    constexpr const std::array<sample_t, TABLE_SIZE> makeSineTable(){
        return _makeSineTable<sample_t, TABLE_SIZE>(Indices{});
    }

    template<typename sample_t, int TABLE_SIZE, std::size_t... INDEX>
    constexpr const std::array<sample_t, TABLE_SIZE> _makeCosTable(const std::index_sequence<INDEX...> index_seq){
       
        const sample_t phase_increment = 2.0*M_PI/sample_t(TABLE_SIZE);
        return {{_cos<sample_t>(sample_t(INDEX)*phase_increment)...}};

    }

    template<typename sample_t, int TABLE_SIZE, typename Indices = std::make_index_sequence<TABLE_SIZE>>
    constexpr const std::array<sample_t, TABLE_SIZE> makeCosTable(){
        return _makeCosTable<sample_t,TABLE_SIZE>(Indices{});
    }

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

    template<typename sample_t, size_t TABLE_SIZE>
    struct sine_t{
        static constexpr std::array<sample_t, TABLE_SIZE> TABLE = makeSineTable<sample_t,TABLE_SIZE>();
        inline static const sample_t lookup(const sample_t& phase){
            return table_lookup_lin_interp<sample_t,TABLE_SIZE>(TABLE, phase);
        }

    };

    template<typename sample_t>
    struct sine_t<sample_t,0>{
        inline static const sample_t lookup(const sample_t& phase){
            return sin(TWO_PI*phase);
        }

    };

    template<typename sample_t, size_t TABLE_SIZE, size_t BLOCK_SIZE>
    struct sineOsc{
        inline static const std::pair<sample_t, AudioBlock<sample_t,BLOCK_SIZE>>process(sample_t phase, const sample_t& phi){
            AudioBlock<sample_t, BLOCK_SIZE> output;
            for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                output[idx] = sine_t<sample_t,TABLE_SIZE>::lookup(phase);
                phase = update_phase<sample_t,sample_t>(phase, phi, 1.0);
            }
            return std::pair(phase, output);
        }
    };

    template<typename sample_t, int TABLE_SIZE>
    struct cos_t{
        static constexpr std::array<sample_t, TABLE_SIZE> TABLE = makeCosTable<sample_t,TABLE_SIZE>();
        inline static const sample_t lookup(const sample_t& phase){
            return table_lookup_lin_interp<sample_t,TABLE_SIZE>(TABLE, phase);
        }
        
    };

    template<typename sample_t>
    struct cos_t<sample_t, 0>{
        inline static const sample_t lookup(const sample_t& phase){
            return cos(TWO_PI*phase);
        }
    };

    template<typename sample_t>
    const sample_t noise(){
        return  2.0 * (( rand() /  RAND_MAX) - 0.5);
    }

    template<typename sample_t, size_t BLOCKSIZE>
    const AudioBlock<sample_t,BLOCKSIZE> noise(){
        AudioBlock<sample_t,BLOCKSIZE> output;
        for(size_t idx=0; idx<BLOCKSIZE; idx++){
            output[idx] = noise<sample_t>();
        }
        return output;
    }

    template<typename sample_t>
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
    const inline stk_blit_saw_t<sample_t> setFrequency(stk_blit_saw_t<sample_t> saw, const frequency_t& frequency, const frequency_t& sampleRate){

        saw.p = sampleRate/frequency;
        saw.c2 = 1/saw.p;
        saw.phase_increment = M_PI*saw.c2;

        int max_harmonics = floor(0.5*saw.p);
        saw.m = 2*max_harmonics+1;
        saw.a = saw.m / saw.p;

        return saw;
    }
    
    template<typename sample_t, typename frequency_t>
    const inline stk_blit_saw_t<sample_t> stk_blit_saw(const frequency_t& frequency, const frequency_t& sampleRate=44100){
        stk_blit_saw_t<sample_t> saw;

        saw = setFrequency(saw, frequency, sampleRate);
        saw.state = -0.5 * saw.a;
        return saw;
    }

    template<typename sample_t>
    const inline stk_blit_saw_t<sample_t> process(stk_blit_saw_t<sample_t> saw){
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

    template<typename sample_t, size_t BLOCKSIZE>
    const inline std::pair<stk_blit_saw_t<sample_t>,AudioBlock<sample_t,BLOCKSIZE>> process(stk_blit_saw_t<sample_t> saw) {
        
        AudioBlock<sample_t,BLOCKSIZE> output;
        for(size_t idx=0;idx<BLOCKSIZE;idx++){
            saw = process<sample_t>(saw);
            output[idx] = saw.state;
        }
        
        return std::pair(saw,output);
    }

    template<typename sample_t>
    struct stk_blit_square_t{
        int m;
        sample_t phase_increment;
        sample_t phase=0;
        sample_t p;
        sample_t a;
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


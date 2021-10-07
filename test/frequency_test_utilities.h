#pragma once
#include <fftw3.h>
#include <tuple>
#include "../src/core/audio_block.h"
#include "../src/core/oscillator/phasor.h"
#include "../src/core/oscillator/sin_table_lookup.h"
#include "../src/core/oscillator/cos_table_lookup.h"

using algae::dsp::core::AudioBlock;
using algae::dsp::core::oscillator::noise;
using algae::dsp::core::oscillator::compute_phase_increment;
using algae::dsp::core::oscillator::update_phase;
using algae::dsp::core::oscillator::sineOsc;
using algae::dsp::core::TWO_PI;
using algae::dsp::core::filter::process;
template<typename sample_t, typename filter_t, size_t NUM_BINS, size_t FRAME_SIZE>
std::array<sample_t, NUM_BINS> compute_amplitude_response(filter_t filter, sample_t sample_rate){
    std::array<sample_t, NUM_BINS> amplitude_response={};
    // for(size_t bin_idx=0; bin_idx<NUM_BINS; bin_idx++){
    //     filter_t filter = flt;
    //     sample_t freq = sample_t(bin_idx) * (sample_rate/sample_t(NUM_BINS));
    //     sample_t phase_increment = compute_phase_increment(freq,sample_rate);
    //     sample_t phase = 0;
    //     AudioBlock<sample_t, FRAME_SIZE> sine_wave;
    //     std::tie(phase, sine_wave) = sineOsc<sample_t, 1024, FRAME_SIZE>::process(phase, phase_increment);
    //     AudioBlock<sample_t, FRAME_SIZE> filtered_block;
    //     std::tie(filter,filtered_block) = process(filter, sine_wave);

    //     sample_t max_sample=0;
    //     for(size_t frame_idx=0; frame_idx<FRAME_SIZE; frame_idx++){
    //         sample_t filtered_sample = filtered_block[frame_idx];
    //         if(filtered_sample>max_sample) max_sample=filtered_sample;
    //     }

    //     amplitude_response[bin_idx]=max_sample;
        
    // }
    // fftw_plan fftw_plan_dft_r2c_1d(int n, double *in, fftw_complex *out,
    //                            unsigned flags);
    enum complex {RE, IM};
    constexpr size_t N = 2*NUM_BINS;
    constexpr size_t M = NUM_BINS;//2*NUM_BINS -1;
    fftw_complex * in;
    fftw_complex *out;
    fftw_plan p;
   
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    // p = fftw_plan_dft_r2c_1d(N, in, out, /*FFTW_FORWARD,*/ FFTW_ESTIMATE);
    
            

    AudioBlock<sample_t, N> nz = AudioBlock<sample_t, N>();
    nz[0] = 1;
    // AudioBlock<sample_t, N> nz = noise<sample_t, N>();
    std::tie(filter,nz) = process(filter, nz);
    for(size_t idx=0; idx<N; idx++){
        in[idx][RE]= nz[idx];
        in[idx][IM]= 0;

    }



    // void fftw_execute(const fftw_plan plan);
    
    fftw_execute(p); /* repeat as needed */

    for(size_t idx=0; idx<NUM_BINS; idx++)
        amplitude_response[idx] = sqrt(out[idx][RE]*out[idx][RE] + out[idx][IM]*out[idx][IM]);    

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

    return amplitude_response;
}
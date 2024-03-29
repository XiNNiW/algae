#pragma once
#include "algae.h"
#include <fftw3.h>
#include <tuple>

template <typename sample_t, typename filter_t, size_t NUM_BINS>
const std::array<sample_t, NUM_BINS>
compute_amplitude_response(filter_t filter, const sample_t sample_rate) {
  std::array<sample_t, NUM_BINS> amplitude_response = {};

  enum complex { RE, IM };
  const size_t N = 2 * NUM_BINS;
  fftw_complex *in;
  fftw_complex *out;
  fftw_plan p;

  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  sample_t impulse[N];
  algae::dsp::block::empty(N, impulse);
  impulse[0] = 1;
  filter.process(impulse, N, impulse);
  for (size_t idx = 0; idx < N; idx++) {
    in[idx][RE] = impulse[idx];
    in[idx][IM] = 0;
  }

  fftw_execute(p);

  for (size_t idx = 0; idx < NUM_BINS; idx++) {
    amplitude_response[idx] =
        (sqrt(out[idx][RE] * out[idx][RE] + out[idx][IM] * out[idx][IM]));
  }

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);

  return amplitude_response;
}

template <typename sample_t, size_t NUM_BINS>
const std::array<sample_t, NUM_BINS> compute_whole_spectrum_magnitude_in_db(
    const std::array<sample_t, 2 * NUM_BINS> &time_data,
    const sample_t &sample_rate) {
  std::array<sample_t, NUM_BINS> spectrum = {};
  enum complex { RE, IM };
  const size_t N = 2 * NUM_BINS;
  fftw_complex *in;
  fftw_complex *out;
  fftw_plan p;

  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  for (size_t idx = 0; idx < N; idx++) {

    in[idx][RE] = time_data[idx];
    in[idx][IM] = 0;
  }

  fftw_execute(p);

  sample_t max_magnitude = 0;
  for (size_t idx = 0; idx < NUM_BINS; idx++) {
    spectrum[idx] =
        (sqrt(out[idx][RE] * out[idx][RE] + out[idx][IM] * out[idx][IM]));
    if (spectrum[idx] > max_magnitude) {
      max_magnitude = spectrum[idx];
    }
  }

  max_magnitude = max_magnitude > 0 ? max_magnitude : 1;

  for (size_t idx = 0; idx < NUM_BINS; idx++) {
    spectrum[idx] = 10 * log10(spectrum[idx] / max_magnitude);
  }

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);

  return spectrum;
}

template <typename sample_t, size_t NUM_BINS>
const std::array<sample_t, NUM_BINS>
compute_whole_spectrum_magnitude_in_db(const sample_t *time_data,
                                       const sample_t &sample_rate) {
  std::array<sample_t, NUM_BINS> spectrum = {};
  enum complex { RE, IM };
  const size_t N = 2 * NUM_BINS;
  fftw_complex *in;
  fftw_complex *out;
  fftw_plan p;

  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  for (size_t idx = 0; idx < N; idx++) {

    in[idx][RE] = time_data[idx];
    in[idx][IM] = 0;
  }

  fftw_execute(p);

  sample_t max_magnitude = 0;
  for (size_t idx = 0; idx < NUM_BINS; idx++) {
    spectrum[idx] =
        (sqrt(out[idx][RE] * out[idx][RE] + out[idx][IM] * out[idx][IM]));
    if (spectrum[idx] > max_magnitude) {
      max_magnitude = spectrum[idx];
    }
  }

  max_magnitude = max_magnitude > 0 ? max_magnitude : 1;

  for (size_t idx = 0; idx < NUM_BINS; idx++) {
    spectrum[idx] = 10 * log10(spectrum[idx] / max_magnitude);
  }

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);

  return spectrum;
}


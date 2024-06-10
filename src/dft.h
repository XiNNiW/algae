#pragma once

#include "src/algae.h"
namespace algae::dsp::analysis {

template <typename sample_t>
void dft(sample_t *xRe, sample_t *xIm, sample_t *yRe, sample_t *yIm,
         size_t size) {

  for (size_t k = 0; k < size; k++) {

    sample_t yReSum = 0;
    sample_t yImSum = 0;
    for (size_t n = 0; n < size; n++) {
      yReSum += xRe[n] * cos(math::TwoPi<sample_t>() * k * n / size);
      yImSum += -xIm[n] * sin(math::TwoPi<sample_t>() * k * n / size);
    }

    yRe[k] = yReSum;
    yIm[k] = yImSum;
  }
}

} // namespace algae::dsp::analysis

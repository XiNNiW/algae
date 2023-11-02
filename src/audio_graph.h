#pragma once
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};                                                             // (1)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>; // (2)
// #include <gl\glew.h>
// #include <gl\glu.h>
// #include <stdio.h>
// #include <string>
template <typename sample_t> class Synthesizer {

  void next(sample_t &leftOut, sample_t &rightOut) {
    leftOut = 0;
    rightOut = 0;
  }
};

using algae::dsp::_StereoGenerator;
using algae::dsp::Generator;
using algae::dsp::StereoGenerator;
using algae::dsp::UGen;
template <typename sample_t, size_t NUM_INPUTS>
class StereoMixer
    : _StereoGenerator<sample_t, StereoMixer<sample_t, NUM_INPUTS>> {
  std::array<UGen<sample_t> *, NUM_INPUTS> sources;
  const inline void next(sample_t *outL, sample_t *outR) {
    for (size_t i = 0; i < NUM_INPUTS; i++) {
      std::visit(overloaded{
                     [](Generator<sample_t> &g) {

                     },
                     [](StereoGenerator<sample_t> &st) {

                     },
                 },
                 *sources.at(i));
    }
  }
};

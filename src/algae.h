#pragma once
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <utility>
#include <variant>

namespace algae::dsp {

template <typename sample_t> struct Generator {
  virtual const inline sample_t next() { return 0; };
};

template <typename sample_t> struct StereoGenerator {
  virtual const inline void next(sample_t *outL, sample_t *outR) {
    outL = 0;
    outR = 0;
  };
};
template <typename sample_t> struct Filter {
  virtual const inline sample_t next(sample_t in) { return 0; };
};

template <typename sample_t> struct StereoFilter {
  virtual const inline void next(sample_t inL, sample_t inR, sample_t *outL,
                                 sample_t *outR) {
    outL = 0;
    outR = 0;
  };
};

template <typename sample_t, typename DerivedT>
struct _Filter : Filter<sample_t> {
  virtual inline void process(const sample_t *in, const size_t blocksize,
                              sample_t *out) {
    for (size_t i = 0; i < blocksize; i++)
      out[i] = static_cast<DerivedT *>(this)->next(in[i]);
  }
};

template <typename sample_t, typename DerivedT>
struct _StereoFilter : StereoFilter<sample_t> {
  inline void process(const sample_t *inL, const sample_t *inR,
                      const size_t blocksize, sample_t *outL, sample_t *outR) {
    for (size_t i = 0; i < blocksize; i++)
      static_cast<DerivedT *>(this)->next(inL[i], inR[i], outL[i], outR[i]);
  }
};
template <typename sample_t, typename DerivedT>
struct _Generator : Generator<sample_t> {
  inline void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++)
      out[i] = static_cast<DerivedT *>(this)->next();
  }
};
template <typename sample_t, typename DerivedT>
struct _StereoGenerator : StereoGenerator<sample_t> {
  inline void process(const size_t blocksize, sample_t *outL, sample_t *outR) {
    for (size_t i = 0; i < blocksize; i++) {
      static_cast<DerivedT *>(this)->next(outL[i], outR[i]);
    }
  }
};
// template <typename sample_t, typename DerivedT, size_t NUM_CHANNELS>
// struct MultichannelFilter {
//   inline void next(const sample_t **inputs, const sample_t **outputs) = 0;
// };
// enum class UGenType {
//  MONO_GENERATOR,
//  STEREO_GENERATOR,
//  MONO_FILTER,
//  STEREO_FILTER
//};
// template <typename sample_t> struct UGen {
//   UGenType type;
//   union {
//     Generator<sample_t> g;
//     StereoGenerator<sample_t> sg;
//     Filter<sample_t> f;
//     StereoFilter<sample_t> sf;
//   } _node;
//   template <typename T>
//   const inline T &asType(){
//
//   };
//
//   const static inline UGen &asUGen(const Generator<sample_t> &g) {
//     return UGen{._node = g, .type = UGenType::MONO_GENERATOR};
//   };
//   const static inline UGen &asUGen(const Filter<sample_t> &g) {
//     return UGen{._node = g, .type = UGenType::MONO_FILTER};
//   };
//   const static inline UGen &asUGen(const StereoGenerator<sample_t> &g) {
//     return UGen{._node = g, .type = UGenType::STEREO_GENERATOR};
//   };
//   const static inline UGen &asUGen(const StereoFilter<sample_t> &g) {
//     return UGen{._node = g, .type = UGenType::STEREO_FILTER};
//   };
//
//   template <typename T> const bool isType() { return false; }
//
//   template <> const bool isType<Generator<sample_t>>() {
//     return type == UGenType::MONO_GENERATOR;
//   }
//   template <> const bool isType<StereoGenerator<sample_t>>() {
//     return type == UGenType::STEREO_GENERATOR;
//   }
//   template <> const bool isType<StereoFilter<sample_t>>() {
//     return type == UGenType::STEREO_FILTER;
//   }
//   template <> const bool isType<Filter<sample_t>>() {
//     return type == UGenType::MONO_FILTER;
//   }
// };

// template <typename sample_t>
// struct UGen : std::variant<Generator<sample_t>, StereoGenerator<sample_t>,
//                            Filter<sample_t>, StereoFilter<sample_t>> {};

// template <typename sample_t, size_t NUM_INPUTS, size_t NUM_OUTPUTS>
// struct Node : UGen<sample_t> {
//   std::array<Node, NUM_INPUTS> inputNodes;
//   std::array<Node, NUM_OUTPUTS> outputNodes;
//   const inline void update(sample_t);
// };
namespace math {
// CONSTANTS
#define LOGTEN 2.302585092994046
#define HALF_PI = M_PI_2;

template <typename sample_t> constexpr sample_t TwoPi() {
  return static_cast<sample_t>(2 * M_PI);
}

template <typename sample_t> constexpr sample_t HalfPi() {
  return static_cast<sample_t>(M_PI_2);
}

template <typename sample_t> constexpr sample_t Pi() {
  return static_cast<sample_t>(M_PI);
}

// FUNCTIONS

constexpr size_t nextPowerOf2(const size_t &x) {
  int next = 1;
  while (next < x) {
    next = next << 1;
  }
  return next;
  // return static_cast<size_t>(pow(2, ceil(log(x) / log(2))));
};

template <typename sample_t, unsigned int x> struct factorial_t {
  static constexpr sample_t result = x * factorial_t<sample_t, x - 1>::result;
};

template <typename sample_t> struct factorial_t<sample_t, 0> {
  static constexpr sample_t result = 1;
};

template <typename sample_t> inline const sample_t d_sqrt(sample_t n) {
  int i;
  float x, y;
  x = n * 0.5;
  y = n;
  i = *(int *)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float *)&i;
  y = y * (1.5 - (x * y * y));
  y = y * (1.5 - (x * y * y));
  return n * y;
};

template <typename sample_t>
static const inline sample_t lerp(const sample_t origin, const sample_t dest,
                                  const sample_t amt) {
  sample_t a = (amt > 1) ? 1 : (amt < 0) ? 0 : amt;
  return origin + a * (dest - origin);
};

template <typename sample_t>
static const inline sample_t clip(const sample_t x, const sample_t lowerBound,
                                  const sample_t upperBound) {
  return (x > upperBound) ? upperBound : (x < lowerBound) ? lowerBound : x;
};

template <typename sample_t>
static const inline sample_t clip(const sample_t x) {
  return clip<sample_t>(x, -1, 1);
};

template <typename sample_t>
static inline void clip(const sample_t *x, const size_t blocksize,
                        sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = clip<sample_t>(x[i]);
  }
};

template <typename sample_t>
static const inline sample_t wrap(const sample_t x) {
  return x - floor(x);
};

template <typename sample_t>
static inline void wrap(const sample_t *x, const size_t blocksize,
                        sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = wrap<sample_t>(x[i]);
  }
};

template <typename sample_t>
static const inline sample_t fmod(const sample_t x, const sample_t y) {
  return wrap(x / y) * y;
};

template <typename sample_t>
static inline void fmod(const sample_t *x, const sample_t *y,
                        const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = fmod<sample_t>(x[i]);
  }
};

template <typename sample_t>
static const inline sample_t clamp(const sample_t x, const sample_t lo,
                                   const sample_t hi) {
  return fmin(fmax(x, lo), hi);
};

template <typename sample_t>
static const inline void clamp(const sample_t *x, const sample_t *lo,
                               const sample_t *hi, const size_t blocksize,
                               sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = clamp(x[i], lo[i], hi[i]);
  }
};

template <typename sample_t>
static const inline sample_t atan(const sample_t x) {
  return std::atan(x);
};

template <typename sample_t>
static inline void atan(const sample_t *x, const size_t blocksize,
                        sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = atan(x[i]);
  }
};

template <typename sample_t>
static inline const sample_t tanh(const sample_t x) {
  return std::tanh(x);
}

template <typename sample_t>
static inline sample_t tanh_approx_pade(const sample_t x) {
  if (x < -3)
    return -1;
  else if (x > 3)
    return 1;
  else
    return x * (27 + x * x) / (27 + 9 * x * x);
};

template <typename sample_t>
static const inline void
tanh_approx_pade(const sample_t *x, const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = tanh_approx_pade(x[i]);
  }
};

template <typename sample_t>
static const inline sample_t tanh_approx_pade_no_clip(const sample_t x) {
  return x * (27 + x * x) / (27 + 9 * x * x);
};

template <typename sample_t>
static inline void tanh_approx_pade_no_clip(const sample_t *x,
                                            const size_t blocksize,
                                            const sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = tanh_approx_pade(x[i]);
  }
};

template <typename sample_t>
static const inline sample_t lowered_bell(const sample_t x) {
  return 2.0 / (x * x + 1.0) - 1.0;
};

template <typename sample_t>
static inline void lowered_bell(const sample_t *x, const size_t blocksize,
                                sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lowered_bell(x[i]);
  }
};

// UNIT CONVERSION
template <typename sample_t>
inline const sample_t beatsToSamples(sample_t n, sample_t bpm,
                                     sample_t sampleRate) {
  const sample_t SECONDS_PER_MINUTE = 60;
  return (n / bpm) * SECONDS_PER_MINUTE * sampleRate;
};

template <typename sample_t>
inline const sample_t beatsToMillis(sample_t n, sample_t bpm) {
  const sample_t SECONDS_PER_MINUTE = 60;
  const sample_t MILLIS_PER_SEC = 1000.0;
  return (n / bpm) * SECONDS_PER_MINUTE * MILLIS_PER_SEC;
};

// implementation borrowed from Miller Puckette's Pure Data project
template <typename sample_t> const inline sample_t dbtopow(sample_t db) {
  if (db <= 0)
    return (0);
  else {
    if (db > 870)
      db = 870;
    return (exp((LOGTEN * 0.1) * (db - 100.)));
  }
};

// implementation borrowed from Miller Puckette's Pure Data project
template <typename sample_t> const inline sample_t dbtorms(sample_t db) {
  if (db <= 0) {
    return (0);
  } else {
    if (db > 485)
      db = 485;
  }
  return (exp((LOGTEN * 0.05) * (db - 100.)));
}

template <typename sample_t>
sample_t msToSamples(sample_t ms, sample_t sampleRate) {
  const sample_t SECONDS_PER_MILLISECOND = 1 / 1000.0;
  return ms * sampleRate * SECONDS_PER_MILLISECOND;
}

template <typename frequency_t>
inline constexpr frequency_t mtof(const frequency_t n) {
  const frequency_t REF_FREQ = 440.0;
  const int REF_NOTE = 69;
  return REF_FREQ * pow(2, (n - REF_NOTE) / 12);
}

// implementation borrowed from Miller Puckette's Pure Data project
template <typename sample_t> const inline sample_t rmstodb(sample_t rms) {
  if (rms <= 0)
    return (0);
  else {
    sample_t val = 100 + 20. / LOGTEN * log(rms);
    return (val < 0 ? 0 : val);
  }
}

// a version of Thomas Wang's integer hash
inline const int32_t hash(int32_t in) {
  uint32_t hash = (uint32_t)in;
  hash += ~(hash << 15);
  hash ^= hash >> 10;
  hash += hash << 3;
  hash ^= hash >> 6;
  hash += ~(hash << 11);
  hash ^= hash >> 16;
  return (uint32_t)hash;
}

} // namespace math
namespace block {
// SAMPLE BLOCK OPERATIONS
template <typename sample_t, const sample_t (*fn)(const sample_t)>
static inline void unaryOp(const sample_t *in, const size_t blocksize,
                           sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = fn(in[i]);
  }
}

template <typename sample_t,
          const sample_t (*fn)(const sample_t, const sample_t)>
static inline void binaryOp(const sample_t *lhs, const sample_t *rhs,
                            const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = fn(lhs[i], rhs[i]);
  }
}
template <typename sample_t,
          const sample_t (*fn)(const sample_t, const sample_t)>
static inline void binaryOp(const sample_t lhs, const sample_t *rhs,
                            const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = fn(lhs, rhs[i]);
  }
}
template <typename sample_t,
          const sample_t (*fn)(const sample_t, const sample_t)>
static inline void binaryOp(const sample_t *lhs, const sample_t rhs,
                            const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = fn(lhs[i], rhs);
  }
}

template <typename sample_t>
static inline void fill(sample_t value, const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = value;
  }
}

template <typename sample_t>
static inline void empty(const size_t blocksize, sample_t *out) {
  fill<sample_t>(0, blocksize, out);
};

template <typename sample_t>
static inline void mult(const sample_t *lhs, const sample_t *rhs,
                        const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs[i] * rhs[i];
  }
}

template <typename sample_t>
static inline void mult(const sample_t lhs, const sample_t *rhs,
                        const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs * rhs[i];
  }
};

template <typename sample_t>
static inline void mult(const sample_t *lhs, const sample_t rhs,
                        const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs[i] * rhs;
  }
};

template <typename sample_t>
static inline void div(const sample_t *lhs, const sample_t *rhs,
                       const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs[i] / rhs[i];
  }
};

template <typename sample_t>
static inline void div(const sample_t lhs, const sample_t *rhs,
                       const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs / rhs[i];
  }
};

template <typename sample_t>
static inline void div(const sample_t *lhs, const sample_t rhs,
                       const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs[i] / rhs;
  }
};

template <typename sample_t>
static inline void add(const sample_t *lhs, const sample_t *rhs,
                       const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs[i] + rhs[i];
  }
};

template <typename sample_t>
static inline void add(const sample_t lhs, const sample_t *rhs,
                       const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs + rhs[i];
  }
};

template <typename sample_t>
static inline void add(const sample_t *lhs, const sample_t rhs,
                       const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs[i] + rhs;
  }
};

template <typename sample_t>
static inline void sub(const sample_t *lhs, const sample_t *rhs,
                       const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs[i] - rhs[i];
  }
};

template <typename sample_t>
static inline void sub(const sample_t lhs, const sample_t *rhs,
                       const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs - rhs[i];
  }
};

template <typename sample_t>
static inline void sub(const sample_t *lhs, const sample_t rhs,
                       const size_t blocksize, sample_t *out) {
  for (size_t i = 0; i < blocksize; i++) {
    out[i] = lhs[i] - rhs;
  }
};

} // namespace block
namespace filter {

template <typename sample_t, size_t MAX_DELAY_SAMPS,
          const sample_t (*interp)(const sample_t, const sample_t,
                                   const sample_t) =
              algae::dsp::math::lerp<sample_t>>
struct InterpolatedDelay
    : _Filter<sample_t, InterpolatedDelay<sample_t, MAX_DELAY_SAMPS, interp>> {
  static const size_t BUFFER_SIZE =
      algae::dsp::math::nextPowerOf2(MAX_DELAY_SAMPS);
  static constexpr int INDEX_MASK = int(BUFFER_SIZE - 1);
  sample_t buffer[BUFFER_SIZE];
  int writePosition = 0;
  sample_t delayTimeSamples = BUFFER_SIZE / 2;

  InterpolatedDelay() { algae::dsp::block::empty(BUFFER_SIZE, buffer); }
  void setDelayTimeMillis(sample_t delayTimeMillis, sample_t samplerate) {
    delayTimeSamples = (delayTimeMillis * samplerate) / 1000.0;
  }

  inline const sample_t tap(int pos) {
    return buffer[(writePosition - pos) & INDEX_MASK];
  }

  inline const sample_t next(const sample_t in) {
    int dtWhole = floor(delayTimeSamples);
    sample_t dtMantissa = delayTimeSamples - dtWhole;
    int r1 = (writePosition - dtWhole) & INDEX_MASK;
    int r2 = (r1 + 1) & INDEX_MASK;
    sample_t out = interp(buffer[r1], buffer[r2], dtMantissa);
    buffer[writePosition] = in;
    writePosition++;
    writePosition &= INDEX_MASK;
    return out;
  }
};

// inspired by CParamSmooth from moc.liamg@earixela at musicdsp.org
template <typename sample_t> struct SmoothParameter {
  sample_t a = 0.99f, b = 1.f - a, z = 0;

  SmoothParameter() {}
  SmoothParameter(sample_t smoothingTimeMillis, sample_t samplerate) {
    set(smoothingTimeMillis, samplerate);
  }
  void set(sample_t smoothingTimeMillis, sample_t samplerate) {

    a = exp(-math::TwoPi<sample_t>() /
            (smoothingTimeMillis * 0.001 * samplerate));
    b = 1.0 - a;
    // z = 0.0;
  }

  inline const sample_t next(const sample_t in) {
    z = (in * b) + (z * a);
    return z;
  }

  void process(const sample_t in, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next(in);
    }
  }

  void process(const sample_t *in, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++)
      out[i] = next(in[i]);
  }
};
template <typename sample_t, typename frequency_t>
struct Biquad : _Filter<sample_t, Biquad<sample_t, frequency_t>> {

  sample_t b0 = 0, b1 = 0, b2 = 0, a1 = 0, a2 = 0;
  sample_t y1 = 0;
  sample_t y2 = 0;
  sample_t x1 = 0;
  sample_t x2 = 0;

  void setCoefficients(const sample_t b0, const sample_t b1, const sample_t b2,
                       const sample_t a1, const sample_t a2) {
    this->b0 = b0;
    this->b1 = b1;
    this->b2 = b2;
    this->a1 = a1;
    this->a2 = a2;
  };

  const sample_t next(const sample_t in) {
    const sample_t b0 = this->b0;
    const sample_t b1 = this->b1;
    const sample_t b2 = this->b2;
    const sample_t a1 = this->a1;
    const sample_t a2 = this->a2;
    const sample_t x0 = in;
    const sample_t x1 = this->x1;
    const sample_t x2 = this->x2;
    const sample_t y1 = this->y1;
    const sample_t y2 = this->y2;
    const sample_t y0 = b0 * x0 + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
    this->y1 = y0;
    this->y2 = y1;
    this->x1 = x0;
    this->x2 = x1;
    return y0;
  }

  void lowpass(const frequency_t cutoff, const frequency_t quality,
               const frequency_t samplerate) {

    const frequency_t _q = quality <= 0 ? 0.001 : quality;
    const frequency_t _w0 = cutoff * 2 * M_PI / samplerate;
    const frequency_t _a = fabs(sin(_w0) / (2 * _q));
    const frequency_t _c = cos(_w0);
    const frequency_t _b0 = (1 - _c) / 2, _b1 = 1 - _c, _b2 = (1 - _c) / 2;
    const frequency_t _a0 = 1 + _a, _a1 = -2 * _c, _a2 = 1 - _a;

    this->b0 = _b0 / _a0;
    this->b1 = _b1 / _a0;
    this->b2 = _b2 / _a0;
    this->a1 = _a1 / _a0;
    this->a2 = _a2 / _a0;
  };

  void highpass(const frequency_t cutoff, const frequency_t quality,
                const frequency_t samplerate) {

    const frequency_t q = quality <= 0 ? 0.001 : quality;
    frequency_t w0 = cutoff * math::TwoPi<frequency_t>() / samplerate;
    frequency_t a = fabs(sin(w0) / (2 * q));
    frequency_t c = cos(w0);
    frequency_t b0 = (1 + c) / 2;
    frequency_t b1 = -(1 + c);
    frequency_t b2 = (1 + c) / 2;
    frequency_t a0 = 1 + a;
    frequency_t a1 = -2 * c;
    frequency_t a2 = 1 - a;

    this->b0 = b0 / a0;
    this->b1 = b1 / a0;
    this->b2 = b2 / a0;
    this->a1 = a1 / a0;
    this->a2 = a2 / a0;
  };

  void bandpass(const frequency_t cutoff, const frequency_t quality,
                const frequency_t samplerate) {

    const frequency_t q = quality <= 0 ? 0.001 : quality;
    frequency_t w0 = cutoff * math::TwoPi<frequency_t>() / samplerate;
    frequency_t a = fabs(sin(w0) / (2 * q));
    frequency_t c = cos(w0);
    frequency_t b0 = a;
    frequency_t b1 = 0;
    frequency_t b2 = -a;
    frequency_t a0 = 1 + a;
    frequency_t a1 = -2 * c;
    frequency_t a2 = 1 - a;
    this->b0 = b0 / a0;
    this->b1 = b1 / a0;
    this->b2 = b2 / a0;
    this->a1 = a1 / a0;
    this->a2 = a2 / a0;
  };
};

template <typename sample_t, typename frequency_t>
struct Onepole : _Filter<sample_t, Onepole<sample_t, frequency_t>> {

  sample_t a0 = 0;
  sample_t type_coefficient = 1;
  sample_t y1 = 0;

  void setCoefficients(const sample_t a0) { this->a0 = a0; };
  inline const sample_t next(const sample_t in) {
    const sample_t _y1 = type_coefficient * y1;
    y1 = a0 * (in - _y1) + _y1;
    return y1;
  }

  void lowpass(const frequency_t cutoff, const frequency_t samplerate) {

    sample_t w = M_PI * math::clamp<sample_t>(fabs(-cutoff / samplerate), 0, 1);
    a0 = sin(w);
    type_coefficient = 1;
  };

  void highpass(const frequency_t cutoff, const frequency_t samplerate) {

    sample_t w = M_PI * math::clamp<sample_t>(fabs(cutoff / samplerate), 0, 1);
    a0 = sin(w);
    type_coefficient = -1;
  };
};

template <typename sample_t, typename frequency_t>
struct OnepoleOnezero
    : _Filter<sample_t, OnepoleOnezero<sample_t, frequency_t>> {
  sample_t a1 = 0, b0 = 0;
  sample_t x1 = 0, y1 = 0;
  void highpass(const frequency_t cutoff, const frequency_t samplerate) {

    const frequency_t w =
        math::TwoPi<frequency_t>() *
        math::clamp<sample_t>(fabs(cutoff / samplerate), 0, sample_t(0.5));
    const frequency_t a = (1 - sin(w)) / cos(w);
    const frequency_t b = (1 + a) / 2;

    a1 = a;
    b0 = b;
  };

  inline const sample_t next(const sample_t in) {
    const sample_t a1 = this->a1;
    const sample_t b0 = this->b0;
    const sample_t x0 = in;
    const sample_t x1 = this->x1;
    const sample_t y1 = this->y1;
    const sample_t y = b0 * (x0 - x1) + a1 * y1;
    this->y1 = y;
    this->x1 = x0;
    return y;
  }
};

template <typename sample_t, size_t MAX_DELAY_SAMPS = 48000>
struct Allpass : _Filter<sample_t, Allpass<sample_t>> {
  static const size_t BUFFER_SIZE =
      algae::dsp::math::nextPowerOf2(MAX_DELAY_SAMPS);
  static constexpr int INDEX_MASK = int(BUFFER_SIZE - 1);
  sample_t bufferX[BUFFER_SIZE];
  sample_t bufferY[BUFFER_SIZE];
  int writeIndex = 0;
  sample_t g = 0;
  int delayTimeSamples = MAX_DELAY_SAMPS / 2.0;

  Allpass() {
    algae::dsp::block::empty(BUFFER_SIZE, bufferX);
    algae::dsp::block::empty(BUFFER_SIZE, bufferY);
  }
  void setDelayTimeMillis(sample_t delayTimeMillis, sample_t samplerate) {
    delayTimeSamples = (delayTimeMillis * samplerate) / 1000.0;
  }

  inline const sample_t tap(int pos) {
    return bufferY[(writeIndex - pos) & INDEX_MASK];
  }
  inline const sample_t next(const sample_t in) {
    const int r1 = (writeIndex - delayTimeSamples) & INDEX_MASK;
    const sample_t out = -g * in + bufferX[r1] + g * bufferY[r1];
    bufferX[writeIndex] = in;
    bufferY[writeIndex] = out;
    writeIndex++;
    writeIndex &= INDEX_MASK;
    return out;
  }
};

template <typename sample_t>
struct Allpass1stOrderIIR : _Filter<sample_t, Allpass1stOrderIIR<sample_t>> {
  sample_t y1 = 0;
  sample_t x1 = 0;
  sample_t g1 = 0;

  void setCoefficients(const sample_t g) { this->g = g; };
  const inline sample_t next(const sample_t in) {
    sample_t yn = g1 * in + x1 - g1 * y1;

    y1 = yn;
    x1 = in;
    return yn;
  }
};

template <typename sample_t, size_t MAX_DELAY_SAMPS>
struct Allpass2Comb
    : _Filter<sample_t, Allpass2Comb<sample_t, MAX_DELAY_SAMPS>> {
  static const size_t SIZE = algae::dsp::math::nextPowerOf2(MAX_DELAY_SAMPS);
  static constexpr int INDEX_MASK = int(SIZE - 1);
  sample_t yn[SIZE];
  sample_t xn[SIZE];
  int writeIndex = 0;
  sample_t delayTimeSamples = 1;
  sample_t g = 0.5;
  Allpass2Comb() {
    block::empty(SIZE, yn);
    block::empty(SIZE, xn);
  }
  void set(sample_t delayTimeSamps, sample_t g) {
    this->delayTimeSamples = delayTimeSamps;
    this->g = g;
  }

  void setDelayTimeMillis(sample_t delayTimeMillis, sample_t samplerate) {
    delayTimeSamples = (delayTimeMillis * samplerate) / 1000.0;
  }

  inline const sample_t tap(int pos) {
    return yn[(writeIndex - pos) & INDEX_MASK];
  }
  const inline sample_t next(const sample_t in) {

    int readIndex = (this->writeIndex - this->delayTimeSamples);
    readIndex &= INDEX_MASK;
    const sample_t _yn =
        this->g * in + this->xn[readIndex] - this->g * this->yn[readIndex];
    this->yn[this->writeIndex] = _yn;
    this->xn[this->writeIndex] = in;
    this->writeIndex++;
    this->writeIndex &= INDEX_MASK;
    return _yn;
  }
};

template <typename sample_t, size_t ORDER, size_t SIZE = ORDER - 1>
struct AllpoleNthOrder : _Filter<sample_t, AllpoleNthOrder<sample_t, ORDER>> {
  sample_t input_gain = 1;
  sample_t as[ORDER];
  sample_t ys[ORDER];
  AllpoleNthOrder() {
    block::empty(ORDER, as);
    block::empty(ORDER, ys);
  };
  void setCoefficients(const sample_t *as) {
    for (size_t idx = 0; idx < SIZE; idx++) {
      this->ys[idx] = 0;
      this->as[idx] = as[idx + 1] / as[0];
    }
    input_gain = 1 / as[0];
  };
  const inline sample_t next(const sample_t in) {
    sample_t output = in * input_gain;
    for (size_t idx = SIZE; idx-- > 0;) {
      output -= ys[idx] * as[idx];
      ys[idx] = idx > 0 ? ys[idx - 1] : output;
    }
    return output;
  }
};

template <typename sample_t>
struct DCBlocker : _Filter<sample_t, DCBlocker<sample_t>> {
  sample_t y1;
  sample_t x1;
  const inline sample_t next(const sample_t in) {
    y1 = in - x1 + 0.995 * y1;
    x1 = in;
    return y1;
  }
};

template <typename sample_t, typename frequency_t>
struct ResonantBandpass2ndOrderIIR
    : _Filter<sample_t, ResonantBandpass2ndOrderIIR<sample_t, frequency_t>> {
  sample_t b0d;
  sample_t b1d;
  sample_t b2d;
  sample_t a1d;
  sample_t a2d;
  sample_t x1 = 0;
  sample_t x2 = 0;
  sample_t y1 = 0;
  sample_t y2 = 0;
  void setCoefficients(const sample_t b0, const sample_t b1, const sample_t b2,
                       const sample_t a1, const sample_t a2) {
    b0d = b0;
    b1d = b1;
    b2d = b2;
    a1d = a1;
    a2d = a2;
  };
  void bandpass(const frequency_t freq, const sample_t quality,
                const sample_t gain, const frequency_t samplerate) {
    frequency_t q = quality > 0 ? quality : 0.01;
    frequency_t f = freq > 0 ? freq : 0.01;
    sample_t w = math::TwoPi<sample_t>() * f;
    sample_t a1 = 1.0 / q;
    sample_t a0 = 1.0;
    sample_t b2 = 0.0;
    sample_t b1 = gain;
    sample_t b0 = 0.0;
    sample_t c = 1.0 / tan(0.5 * w / samplerate);
    sample_t csq = c * c;
    sample_t d = a0 + a1 * c + csq;

    b0d = (b0 + b1 * c + b2 * csq) / d;
    b1d = 2 * (b0 - b1 * c + b2 * csq) / d;
    b2d = (b0 - b1 * c + b2 * csq) / d;
    a1d = 2 * (a0 - csq) / d;
    a2d = (a0 - a1 * c + csq) / d;
  };
  const inline sample_t next(const sample_t in) {
    const sample_t xn = in;
    const sample_t yn = b0d * xn + b1d * x1 + b2d * x2 - a1d * y1 - a2d * y2;
    y2 = y1;
    y1 = yn;
    x2 = x1;
    x1 = xn;
    return yn;
  }
};
// based off of distorted bandpass filter design in Non-linear filterrs by
// Risto Holopainen
template <typename sample_t, typename frequency_t> struct DistortedBandpass {

  ResonantBandpass2ndOrderIIR<sample_t, frequency_t> resonator;
  sample_t feedback_amt = 0;
  sample_t chaos_gain = 0;

  void setCoefficients(const sample_t b0, const sample_t b1, const sample_t b2,
                       const sample_t a1, const sample_t a2) {
    resonator.setCoefficients(b0, b1, b2, a1, a2);
  };

  void bandpass(const frequency_t freq, const sample_t q, const sample_t gain,
                const frequency_t samplerate) {
    resonator.bandpass(freq, q, gain, samplerate);
  };

  template <const sample_t (*fn)(const sample_t)>
  void process(const sample_t *in, const size_t blocksize, sample_t *out) {

    const sample_t fb = feedback_amt;
    const sample_t g = chaos_gain;
    for (size_t i = 0; i < blocksize; i++) {
      sample_t fb_signal = fn(fb * resonator.y1);

      fb_signal = in[i] + g * fb_signal;
      fb_signal *= 0.5;
      resonator.process(&fb_signal, 1, &out[i]);
    }
  };
};

template <typename sample_t>
struct BabylonianSqrtFilter
    : _Filter<sample_t, BabylonianSqrtFilter<sample_t>> {
  sample_t y1 = 1;
  inline const sample_t next(const sample_t in) {
    sample_t xn = in + 1 + std::numeric_limits<sample_t>::epsilon();
    sample_t yn = 0.5 * (y1 + (xn / y1));
    y1 = yn;
    return y1 - 1 - std::numeric_limits<sample_t>::epsilon();
  }
};

} // namespace filter

namespace oscillator {

template <typename frequency_t>
const inline frequency_t computePhaseIncrement(const frequency_t frequency,
                                               const frequency_t sample_rate,
                                               const frequency_t period = 1) {
  return period * frequency / sample_rate;
}
template <typename sample_t, typename frequency_t>
struct Phasor : _Generator<sample_t, Phasor<sample_t, sample_t>> {
  sample_t phase = 0;
  sample_t phase_increment = 0;
  void setFrequency(const frequency_t frequency, const frequency_t samplerate) {
    phase_increment = static_cast<sample_t>(frequency / samplerate);
  };

  const inline sample_t next(sample_t pm = 0) {
    phase += phase_increment + pm;
    phase = (phase > 1) ? phase - 1 : phase;
    return phase;
  }

  void process(const sample_t *pm, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next(pm[i]);
    }
  };
};

template <typename sample_t>
struct TriangleLFO : _Generator<sample_t, TriangleLFO<sample_t>> {
  Phasor<sample_t, sample_t> phasor;
  void setFrequency(sample_t frequency, sample_t samplerate) {
    phasor.setFrequency(frequency, samplerate);
  }
  inline const sample_t next() { return 2.0 * phasor.next() - 1.0; }
};
template <typename sample_t, typename frequency_t> class RawSawOsc {
private:
  sample_t phase = 0;
  sample_t phase_increment = 0;
  inline const sample_t next(const sample_t pm) {
    const sample_t out = phase * 2 - 1;
    phase += phase_increment + pm;
    phase = (phase > 1) ? phase - 1 : phase;
    return out;
  }

public:
  void setFrequency(const frequency_t frequency, const frequency_t samplerate) {
    phase_increment = static_cast<sample_t>(frequency / samplerate);
  };

  void process(const sample_t *pm, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next(pm[i]);
    }
  };
  void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next(0);
    }
  };
};
// borrowed from the excellent Synthesis Toolkit (STK) library
// https://ccrma.stanford.edu/software/stk/
template <typename sample_t>
static size_t maxNumberOfHarmonics(const sample_t period_in_samples) {
  return static_cast<size_t>(floor(0.5 * period_in_samples));
}
// borrowed from the excellent Synthesis Toolkit (STK) library
// https://ccrma.stanford.edu/software/stk/
template <typename sample_t>
const sample_t sincM(const sample_t phase, const sample_t M) {
  sample_t denominator = sin(phase);
  bool div_zero = denominator <= std::numeric_limits<sample_t>::epsilon();
  return div_zero ? 1 : sin(M * phase) / (M * denominator);
};
// borrowed from the excellent Synthesis Toolkit (STK) library
// https://ccrma.stanford.edu/software/stk/
template <typename sample_t, typename frequency_t> struct STKBlitImpulseTrain {

  sample_t phase = 0;
  sample_t phase_increment = 0;
  sample_t M = 100;
  void setFrequency(const frequency_t frequency, const frequency_t samplerate) {
    const sample_t f = frequency > 0 ? frequency : 0.0001;
    phase_increment = computePhaseIncrement(f, samplerate, M_PI);
    const sample_t period_in_samples = samplerate / f;
    M = 2 * maxNumberOfHarmonics(period_in_samples) + 1;
  };
  void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = sincM(phase, M);
      phase += phase_increment;
      phase = (phase > M_PI) ? 0 : phase;
    }
  };
  void process(const sample_t *freq, const sample_t samplerate,
               const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      setFrequency(freq[i], samplerate);
      out[i] = sincM(phase, M);

      phase += phase_increment;
      phase = (phase > M_PI) ? phase - M_PI : phase;
    }
  };
};

template <typename sample_t> struct click_t {
  sample_t phase = M_PI;
  void process(const sample_t *trig, const sample_t *width_samps,
               const size_t blocksize, sample_t *out);
};
// borrowed from the excellent Synthesis Toolkit (STK) library
// https://ccrma.stanford.edu/software/stk/
template <typename sample_t, typename frequency_t> struct STKBlitSaw {
  int m = 0;
  sample_t phase_increment = 0;
  sample_t phase = 0;
  sample_t p = 0;
  sample_t c2 = 0;
  sample_t a = 0;
  sample_t state = 0;
  sample_t lastFrame = 0;

  STKBlitSaw(const frequency_t freq, const frequency_t samplerate) {
    setFrequency(freq, samplerate);
    state = -0.5 * a;
  }
  void setFrequency(const frequency_t frequency, const frequency_t samplerate) {
    p = samplerate / frequency;
    c2 = 1.0 / p;
    phase_increment = M_PI * c2;

    int max_harmonics = floor(0.5 * p);
    m = 2.0 * max_harmonics + 1;
    a = m / p;
  };
  void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      sample_t tmp, denominator = sin(phase);
      if (fabs(denominator) <= std::numeric_limits<sample_t>::epsilon())
        tmp = a;
      else {
        tmp = sin(m * phase);
        tmp /= p * denominator;
      }

      tmp += state - c2;
      state = tmp * 0.995;

      phase += phase_increment;
      if (phase >= M_PI)
        phase -= M_PI;

      out[i] = tmp;
      lastFrame = tmp;
    }
  };
};
// borrowed from the excellent Synthesis Toolkit (STK) library
// https://ccrma.stanford.edu/software/stk/
template <typename sample_t, typename frequency_t> struct STKBlitSquare {
  int m = 0;
  sample_t phase_increment = 0;
  sample_t phase = 0;
  sample_t p = 0;
  sample_t a = 0;
  sample_t state = 0;
  sample_t last_blit_output = 0;
  sample_t dc_blocker_x1 = 0;
  void setFrequency(const frequency_t frequency, const frequency_t samplerate) {
    p = 0.5 * samplerate / frequency;
    phase_increment = math::Pi<sample_t>() / p;

    int max_harmonics = floor(0.5 * p);
    m = 2 * (max_harmonics + 1);
    a = m / p;
  };
  void process(const size_t blocksize, sample_t *out) {

    for (size_t i = 0; i < blocksize; i++) {
      sample_t tmp = last_blit_output;
      sample_t denominator = sin(phase);
      if (fabs(denominator) < std::numeric_limits<sample_t>::epsilon())
        if (phase < 0.1 || phase > math::TwoPi<sample_t>() - 0.1)
          last_blit_output = a;
        else
          last_blit_output = -a;
      else {
        last_blit_output = sin(m * phase);
        last_blit_output /= p * denominator;
      }

      last_blit_output += tmp;
      state = last_blit_output - dc_blocker_x1 + 0.999 * state;

      dc_blocker_x1 = last_blit_output;

      phase += phase_increment;
      if (phase >= math::TwoPi<sample_t>())
        phase -= math::TwoPi<sample_t>();
      out[i] = state;
    }
  };
};
template <typename sample_t>
const inline sample_t update_phase(const sample_t phase,
                                   const sample_t increment,
                                   const sample_t period = 1) {
  sample_t nextPhase = phase + period * increment;
  if (nextPhase >= period)
    nextPhase -= period;
  return nextPhase;
};

template <typename sample_t>
const inline sample_t table_lookup_linear_interp(const sample_t *table,
                                                 const int tablesize,
                                                 const sample_t phase) {
  sample_t _phase = phase - floor(phase);
  _phase = _phase < 0 ? _phase + 1 : _phase;
  const sample_t position = _phase * (tablesize - 1);
  const int index = floor(position);
  const int x0 = index;
  int x1 = (index + 1) % (tablesize - 1);
  const sample_t mantissa = position - index;
  const sample_t value = table[x0] + mantissa * (table[x1] - table[x0]);
  return value;
};

template <typename sample_t>
constexpr const sample_t _cos(const sample_t phase) {
  using math::factorial_t;
  sample_t x = phase;
  sample_t x2 = x * x;
  sample_t x4 = x2 * x2;
  sample_t x6 = x2 * x4;
  sample_t x8 = x2 * x6;
  sample_t x10 = x2 * x8;
  sample_t x12 = x2 * x10;
  sample_t x14 = x2 * x12;
  sample_t x16 = x2 * x14;
  sample_t x18 = x2 * x16;

  return 1 - (x2 / factorial_t<sample_t, 2>::result) +
         (x4 / factorial_t<sample_t, 4>::result) -
         (x6 / factorial_t<sample_t, 6>::result) +
         (x8 / factorial_t<sample_t, 8>::result) -
         (x10 / factorial_t<sample_t, 10>::result) +
         (x12 / factorial_t<sample_t, 12>::result) -
         (x14 / factorial_t<sample_t, 14>::result) +
         (x16 / factorial_t<sample_t, 16>::result) -
         (x18 / factorial_t<sample_t, 18>::result);
}

template <typename sample_t, int TABLE_SIZE, std::size_t... INDEX>
constexpr const std::array<sample_t, TABLE_SIZE>
_makeCosTable(const std::index_sequence<INDEX...> index_seq) {

  const sample_t phase_increment = 2.0 * M_PI / sample_t(TABLE_SIZE);
  return {{_cos<sample_t>(sample_t(INDEX) * phase_increment)...}};
}

template <typename sample_t, int TABLE_SIZE,
          typename Indices = std::make_index_sequence<TABLE_SIZE>>
constexpr const std::array<sample_t, TABLE_SIZE> makeCosTable() {
  return _makeCosTable<sample_t, TABLE_SIZE>(Indices{});
}

template <typename sample_t, size_t TABLE_SIZE> struct CosineTable {
  static constexpr std::array<sample_t, TABLE_SIZE> TABLE =
      makeCosTable<sample_t, TABLE_SIZE>();
  inline static const sample_t lookup(const sample_t phase) {
    return table_lookup_linear_interp<sample_t>(TABLE.data(), TABLE_SIZE,
                                                phase);
  };
};

template <typename sample_t> struct CosineTable<sample_t, 0> {
  inline static const sample_t lookup(const sample_t phase) {
    return cos(math::TwoPi<sample_t>() * phase);
  };
};

template <typename sample_t>
constexpr const sample_t _sin(const sample_t phase) {
  using math::factorial_t;
  sample_t x = phase;
  sample_t x2 = x * x;
  sample_t x3 = x2 * x;
  sample_t x5 = x2 * x3;
  sample_t x7 = x2 * x5;
  sample_t x9 = x2 * x7;
  sample_t x11 = x2 * x9;
  sample_t x13 = x2 * x11;
  sample_t x15 = x2 * x13;
  sample_t x17 = x2 * x15;
  sample_t x19 = x2 * x17;

  return x - (x3 / factorial_t<sample_t, 3>::result) +
         (x5 / factorial_t<sample_t, 5>::result) -
         (x7 / factorial_t<sample_t, 7>::result) +
         (x9 / factorial_t<sample_t, 9>::result) -
         (x11 / factorial_t<sample_t, 11>::result) +
         (x13 / factorial_t<sample_t, 13>::result) -
         (x15 / factorial_t<sample_t, 15>::result) +
         (x17 / factorial_t<sample_t, 17>::result) -
         (x19 / factorial_t<sample_t, 19>::result);
}

template <typename sample_t, int TABLE_SIZE, std::size_t... INDEX>
constexpr const std::array<sample_t, TABLE_SIZE>
_makeSineTable(const std::index_sequence<INDEX...> index_seq) {

  const sample_t phase_increment =
      2.0 * math::Pi<sample_t>() / sample_t(TABLE_SIZE);
  return {{_sin<sample_t>(sample_t(INDEX) * phase_increment)...}};
}

template <typename sample_t, int TABLE_SIZE,
          typename Indices = std::make_index_sequence<TABLE_SIZE>>
constexpr const std::array<sample_t, TABLE_SIZE> makeSineTable() {
  return _makeSineTable<sample_t, TABLE_SIZE>(Indices{});
}

template <typename sample_t, int TABLE_SIZE> struct SineTable {
  static constexpr std::array<sample_t, TABLE_SIZE> TABLE =
      makeSineTable<sample_t, TABLE_SIZE>();
  static const sample_t lookup(const sample_t phase) {
    return table_lookup_linear_interp<sample_t>(TABLE.data(), TABLE_SIZE,
                                                phase);
  };
};

template <typename sample_t> struct SineTable<sample_t, 0> {
  static const sample_t lookup(const sample_t phase) {
    return sin(math::TwoPi<sample_t>() * phase);
  };
};

template <typename sample_t, typename frequency_t, size_t TABLE_SIZE = 0>
struct SinOscillator
    : _Generator<sample_t, SinOscillator<sample_t, frequency_t>> {

  sample_t phase = 0;
  sample_t phaseIncrement = 0;

  inline const sample_t next(const sample_t pmod = 0) {
    const sample_t out =
        SineTable<sample_t, TABLE_SIZE>::lookup(math::wrap(phase + pmod));
    phase += phaseIncrement;
    if (phase > 1)
      phase -= 1;

    return out;
  }
  void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next();
    }
  };
  void process(const sample_t *pmod, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next(pmod[i]);
    }
  };
  void setFrequency(const frequency_t frequency, const frequency_t samplerate) {
    phaseIncrement = computePhaseIncrement(frequency, samplerate);
  };
};

template <typename sample_t>
struct FeedbackSinOscillator
    : _Generator<sample_t, FeedbackSinOscillator<sample_t>> {

  sample_t phase = 0;
  sample_t phaseIncrement = 0;
  sample_t y1 = 0;
  sample_t feedback = 0;
  inline const sample_t next(const sample_t pm = 0) {
    // const sample_t out = SineTable<sample_t, TABLE_SIZE>::lookup(
    //     math::wrap(p + ((y1 * feedback) / 2.0 * M_PI)));
    const sample_t out = sin(pm + phase + (y1 * feedback));
    y1 = out;
    phase += phaseIncrement;
    if (phase > 2 * M_PI)
      phase -= 2 * M_PI;

    return out;
  }

  void process(const sample_t *pmod, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next(pmod[i]);
    }
  };
  void process(const sample_t *pmod, const sample_t *fb, const size_t blocksize,
               sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      setFeedback(fb[i]);
      out[i] = next(pmod[i]);
    }
  };
  void setFeedback(const sample_t _fb) { feedback = _fb; }
  void setFrequency(const sample_t frequency, const sample_t samplerate) {
    phaseIncrement = 2 * M_PI * frequency / samplerate;
  };
};

template <typename sample_t> const sample_t noise() {
  return static_cast<sample_t>(2.0 * rand() / (RAND_MAX + 1.0) - 1.0);
}

template <typename sample_t> struct WhiteNoise {
  void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = noise<sample_t>();
    }
  };
  sample_t next() { return noise<sample_t>(); }
};

template <typename sample_t>
struct SmoothRand : _Generator<sample_t, SmoothRand<sample_t>> {
  sample_t phase = 0;
  sample_t phaseIncrement = 440.0 / 48000.0;
  sample_t y1 = 0, y2 = 0;
  void setFrequency(sample_t frequency, sample_t samplerate) {
    phaseIncrement = frequency / samplerate;
  }
  const inline sample_t next() {
    sample_t t = phase * phase * (3 - 2 * phase);
    sample_t yn = lerp(y2, y1, t);
    phase += phaseIncrement;
    if (phase > 1) {
      phase -= 1;
      y2 = y1;
      y1 = noise<sample_t>();
    }
  }
};

template <typename sample_t>
static const inline sample_t blep(const sample_t t, const sample_t dt) {
  if (t < dt) {
    auto x = (t / dt - 1);
    return -x * x;
  } else if (t > (1 - dt)) {
    auto x = (t - 1) / dt + 1;
    return x * x;
  } else {
    return 0;
  }
};

template <typename sample_t>
const inline sample_t blamp(const sample_t &t, const sample_t &dt) {
  if (t < dt) {
    auto x = t / dt - 1;
    return -1 / 3.0 * x * x * x;
  } else if (t > 1 - dt) {
    auto x = (t - 1) / dt + 1;
    return 1 / 3.0 * x * x * x;
  } else {
    return 0;
  }
}

template <typename sample_t, typename frequency_t> struct PolyBLEPSaw {
  sample_t phase_increment = 0;
  sample_t phase = 0;
  void setFrequency(const frequency_t frequency, const frequency_t sampleRate) {

    phase_increment = static_cast<sample_t>(frequency / sampleRate);
  };

  // void process(const sample_t *pm, const size_t blocksize, sample_t *out);
  const inline void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next();
    }
  };

  const inline sample_t next() {
    const sample_t one = 1;
    sample_t next = sample_t(2) * phase - one;
    next -= blep<sample_t>(phase, phase_increment);
    phase += phase_increment;
    phase = phase > 1 ? phase - 1 : phase;
    return next;
  }
};

template <typename sample_t, typename frequency_t> class PolyBLEPSquare {
private:
  const inline sample_t next(const sample_t pwidth = 0.5,
                             const sample_t pm = 0) {

    const sample_t one = 1;
    sample_t next = phase < pwidth ? one : -one;
    next += blep<sample_t>(phase, phase_increment);
    next -= blep<sample_t>(fmod(phase + (one - pwidth), one), phase_increment);

    phase += phase_increment + pm;
    phase = phase > 1 ? phase - 1 : phase;
    return next;
  };

public:
  sample_t phase_increment = 0;
  sample_t phase = 0;
  void setFrequency(const frequency_t frequency, const frequency_t sampleRate) {
    phase_increment = static_cast<sample_t>(frequency / sampleRate);
  };

  void process(const sample_t *pwidth, const sample_t *pm,
               const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next(pwidth[i], pm[i]);
    }
  };
  void process(const sample_t *pwidth, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next(pwidth[i]);
    }
  };
  void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next();
    }
  };
};

template <typename sample_t, typename frequency_t> struct PolyBLEPTri {
private:
  const inline sample_t next(const sample_t pm = 0) {
    const sample_t one = 1;
    const sample_t pwidth = 0.5;
    sample_t out = phase < pwidth ? one : -one;
    out += blep<sample_t>(phase, phase_increment);
    out -= blep<sample_t>(fmod(phase + (one - pwidth), one), phase_increment);
    out = phase_increment * out + (one - phase_increment) * y1;
    y1 = out;
    phase += phase_increment + pm;
    phase = phase > 1 ? phase - 1 : phase;
    return out;
  };

public:
  sample_t phase_increment = 0;
  sample_t phase = 0;
  sample_t y1 = 0;
  void setFrequency(const frequency_t frequency, const frequency_t sampleRate) {
    phase_increment = static_cast<sample_t>(frequency / sampleRate);
  };
  void process(const sample_t *pm, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next(pm[i]);
    }
  };
  void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next();
    }
  };
};

template <typename sample_t>
struct LCGNoise : _Generator<sample_t, LCGNoise<sample_t>> {
  long a = 3;
  long c = 0;
  long m = (1LL << 32);
  long y1 = 2533;
  void set(sample_t a, sample_t c, sample_t m) {
    this->a = a;
    this->c = c;
    this->m = m;
  }
  //  void setFrequency(sample_t frequency, sample_t samplerate) {
  //    long period = floor(samplerate / frequency);
  //    m = period;
  //    c = 1;
  //    a = 1;
  //    long z = 2;
  //    long n = m;
  //    while (z * z <= n) {
  //      if (n % z == 0) {
  //        a *= z;
  //        n /= z;
  //      } else {
  //        z++;
  //      }
  //    }
  //
  //    if (n > 1)
  //      a *= n;
  //  }
  void setSeed(sample_t seed) { y1 = seed; }
  const sample_t next() {
    y1 = (a * y1 + c) % m;
    return 2.0 * (sample_t(y1) / sample_t(m)) - 1.0;
  }
};

template <typename sample_t>
struct TriangleNoiseOsc : _Generator<sample_t, TriangleNoiseOsc<sample_t>> {
  union {
    float f;
    int i;
  } u;
  sample_t phase;
  sample_t phi;
  sample_t mod = (255 << 7);
  sample_t a = 0, b = 0, z = 0, x = 0, y = 0, R = 0.99, prev = 0;
  sample_t mix = 0;
  sample_t frequency = 440;
  sample_t pwidth = 0.5;
  int i = 1;

  void setFrequency(const sample_t frequency, const sample_t samplerate) {
    this->frequency = frequency;
    R = exp(-1.0 / (0.0025 * samplerate));
    phi = static_cast<sample_t>(frequency / samplerate);
    auto period = (samplerate / frequency);
    a = exp(-2 * M_PI / period);
    b = 1.0 - a;
  }
  const inline sample_t polyblep(sample_t dt, sample_t t) {
    if (t < dt) {
      t /= dt;
      return t + t - t * t - 1.0;
    } else if (t > 1.0 - dt) {
      t = (t - 1.0) / dt;
      return t * t + t + t + 1.0;
    }

    return 0.0;
  }
  const sample_t next() {
    // experiment 2 ... noticable pitch and ringing in high harmonics... very
    // aliased
    //  sample_t p = sin(2 * M_PI * phase);
    //  sample_t s1 = math::hash(0);
    //  sample_t s2 = math::hash(mod);
    //  sample_t s = math::lerp(s1, s2, p) + p * math::hash(p * mod);

    // experiement 1
    // with high mod it is a filtered noise... slightly metallic
    // low mod it has a noticable pitch but with lots of aliasing
    // int s = math::hash(phase * mod);

    // experiment 3 ... not smooth in both increase and decrease?
    // u.i = 0x40000000 | ((uint32_t)math::hash(std::ceil(phase) * mod) >> 9);
    // sample_t s = u.f - 3.f;
    // z = (s * b) + z * a;

    // experiment 4 ...

    int key = phase > pwidth ? i : i + 1;
    u.i = 0x40000000 | ((uint32_t)math::hash(sample_t(key) * mod) >> 9);
    sample_t value = u.f - 3.f;
    sample_t height = abs(value);

    /* compute square */
    // value = phase < 0.5 ? 1.0 : -1.0;
    // value += polyblep(phi, phase) * height;
    // value -= polyblep(phi, fmod(phase, 1.0)) * height;

    // HERE
    // value += polyblep(phi, phase);
    // value -= polyblep(phi, fmod(phase + 1.0, 1.0));

    value -= polyblep(phi, phase) * height;
    value += polyblep(phi, fmod(phase + (1 - pwidth), 1.0)) * height;

    /* scale and integrate */
    // value *= (4.0 / this->frequency);
    // value += prev;
    // prev = value;

    sample_t square = value;
    sample_t tri = phi * value + (1 - phi) * prev;
    prev = tri;

    /* dc blocker */
    y = tri - x + R * y;
    x = tri;
    value = math::lerp(tri, square, mix);

    phase += phi;
    if (phase > 1) {
      phase -= 1;
      i++;
    }
    return value;
  }
};

template <typename sample_t>
struct SquareNoiseOsc : _Generator<sample_t, TriangleNoiseOsc<sample_t>> {
  union {
    float f;
    int i;
  } u;
  sample_t phase;
  sample_t phi;
  int i = 1;
  sample_t frequency = 440;
  sample_t mod = 1;

  void setFrequency(const sample_t frequency, const sample_t samplerate) {
    this->frequency = frequency;
    phi = static_cast<sample_t>(frequency / samplerate);
  }
  const inline sample_t polyblep(sample_t dt, sample_t t) {
    if (t < dt) {
      t /= dt;
      return t + t - t * t - 1.0;
    } else if (t > 1.0 - dt) {
      t = (t - 1.0) / dt;
      return t * t + t + t + 1.0;
    }

    return 0.0;
  }
  const sample_t next(sample_t pwidth = 0.5) {

    int key = phase > pwidth ? i : i + 1;

    u.i = 0x40000000 | ((uint32_t)math::hash(sample_t(key) * mod) >> 9);
    sample_t value = u.f - 3.f;
    sample_t height = abs(value);

    // sample_t value = phase > pwidth ? 1 : -1;
    // value -= polyblep(phi, phase);
    // value += polyblep(phi, fmod(phase + (1 - pwidth), 1.0));

    value -= polyblep(phi, phase) * height;
    value += polyblep(phi, fmod(phase + (1 - pwidth), 1.0)) * height;

    phase += phi;
    if (phase > 1) {
      phase -= 1;
      i++;
    }
    return value;
  }
};

template <typename sample_t>
struct SeededNoise : _Generator<sample_t, TriangleNoiseOsc<sample_t>> {
  union {
    float f;
    int i;
  } u;
  sample_t frequency = 440;
  int i = 1;

  const sample_t next() {
    u.i = 0x40000000 | ((uint32_t)math::hash(sample_t(i)) >> 9);
    sample_t value = u.f - 3.f;

    i += 1;
    return value;
  }
};

} // namespace oscillator

namespace control {

template <typename sample_t> struct Ramp {
  sample_t phase = 0;
  sample_t begin;
  sample_t end;
  sample_t ramptime_in_samples;
  sample_t phase_increment;
  void set(const sample_t begin, const sample_t end,
           const sample_t ramptime_in_samples) {
    this->begin = begin;
    this->end = end;
    this->phase_increment = 1.0 / static_cast<sample_t>(ramptime_in_samples);
  };
  void trigger() { phase = 0; };
  void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = phase;
      phase += phase_increment;
      phase = (phase > end) ? end : phase;
    }
  };
  void process(const sample_t *trig, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      if (trig[i] > 0) {
        trigger();
      }
      out[i] = phase;
      phase += phase_increment;
      phase = (phase > end) ? end : phase;
    }
  };
};

template <typename sample_t>
struct Perc : _Generator<sample_t, Perc<sample_t>> {
  sample_t phase = 3;
  sample_t attack_increment = 1.0 / 5.0;
  sample_t decay_increment = 1.0 / 4100.0;
  sample_t curve = 1;
  sample_t last = 0;
  enum Stage { OFF, ATTACK, DECAY } stage = OFF;

  Perc() {}

  Perc(const sample_t attack_ms, const sample_t decay_ms, const sample_t curve,
       const sample_t samplerate) {
    set(attack_ms, decay_ms, curve, samplerate);
  }

  void set(const sample_t attack_ms, const sample_t decay_ms,
           const sample_t curve, const sample_t samplerate) {

    sample_t attack_in_samples = (attack_ms * samplerate) / 1000.0;
    sample_t decay_in_samples = (decay_ms * samplerate) / 1000.0;
    if (attack_in_samples <= 0) {
      attack_in_samples = 1;
    }
    if (decay_in_samples <= 0) {
      decay_in_samples = 1;
    }
    this->attack_increment = 1.0 / attack_in_samples;
    this->decay_increment = 1.0 / decay_in_samples;
    this->curve = curve;
  };

  void trigger() {
    phase = 0;
    stage = Stage::ATTACK;
  };

  inline const sample_t next() {
    sample_t next = 0;
    switch (stage) {
    case Stage::ATTACK: {
      next = pow(curve, 1 - phase) + 0.5 * last;
      last = next;
      phase += attack_increment;
      if (phase > 1) {
        phase = 0;
        stage = Stage::DECAY;
      }
      break;
    }
    case Stage::DECAY: {
      next = pow(curve, phase) + 0.5 * last;
      last = next;
      phase += decay_increment;
      if (phase > 1) {
        phase = 0;
        stage = Stage::OFF;
      }
      break;
    }
    case Stage::OFF: {
      next = 0;
      break;
    }
    }
    return next;
  };

  void process(const sample_t *trig, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      if (trig[i] > 0) {
        trigger();
      }
      out[i] = next();
    }
  };
};

template <typename sample_t>
struct ADEnvelope : _Generator<sample_t, ADEnvelope<sample_t>> {
  enum Stage { OFF, ATTACK, RELEASE } stage = OFF;
  sample_t lastGate = false;
  sample_t attack_increment = 1.0 / 5.0;
  sample_t decay_increment = 1.0 / 4100.0;
  sample_t nextOutput = 0;

  inline const sample_t next(sample_t gate) {
    setGate(gate);
    return next();
  }

  inline const sample_t next() {
    auto out = nextOutput;
    switch (stage) {
    case Stage::ATTACK: {
      nextOutput += attack_increment;
      if (nextOutput > 1) {
        stage = Stage::RELEASE;
        nextOutput = 1;
      }
      break;
    }
    case Stage::RELEASE: {
      nextOutput -= decay_increment;
      if (nextOutput < 0) {
        nextOutput = out = 0;
        stage = Stage::OFF;
      }
      break;
    }
    case Stage::OFF: {
      break;
    }
    }

    return out;
  }

  void set(const sample_t attack_ms, const sample_t decay_ms,
           const sample_t samplerate) {
    sample_t attack_in_samples = (attack_ms * samplerate) / 1000.0;
    sample_t decay_in_samples = (decay_ms * samplerate) / 1000.0;
    if (attack_in_samples <= 0) {
      attack_in_samples = 1;
    }
    if (decay_in_samples <= 0) {
      decay_in_samples = 1;
    }
    attack_increment = 1.0 / attack_in_samples;
    decay_increment = 1.0 / decay_in_samples;
  }

  void trigger() {
    nextOutput = 0;
    stage = Stage::ATTACK;
  }

  inline void setGate(const bool gate) {
    switch (stage) {
    case Stage::OFF: {
      if (gate && !lastGate) {
        nextOutput = 0;
        stage = Stage::ATTACK;
      }
      break;
    }
    case Stage::ATTACK: {
      if (!gate) {

        stage = Stage::RELEASE;
      }
      break;
    }
    case Stage::RELEASE: {
      if (gate && !lastGate) {
        stage = Stage::ATTACK;
      }
      break;
    }
    }
    lastGate = gate;
  }
};

template <typename sample_t>
struct ASREnvelope : _Generator<sample_t, ASREnvelope<sample_t>> {

  enum Stage { OFF, ATTACK, SUSTAIN, RELEASE } stage = OFF;
  sample_t nextOutput = 0;
  sample_t attack_increment = 1.0 / 5.0;
  sample_t decay_increment = 1.0 / 4100.0;
  sample_t lastGate = 0;
  void set(const sample_t attack_ms, const sample_t decay_ms,
           const sample_t samplerate) {

    sample_t attack_in_samples = (attack_ms * samplerate) / 1000.0;
    sample_t decay_in_samples = (decay_ms * samplerate) / 1000.0;
    if (attack_in_samples <= 0) {
      attack_in_samples = 1;
    }
    if (decay_in_samples <= 0) {
      decay_in_samples = 1;
    }
    attack_increment = 1.0 / attack_in_samples;
    decay_increment = 1.0 / decay_in_samples;
  };

  inline void setGate(const bool gate) {
    switch (stage) {
    case Stage::OFF: {
      if (gate && !lastGate) {
        nextOutput = 0;
        stage = Stage::ATTACK;
      }
      break;
    }
    case Stage::ATTACK: {
      if (!gate) {
        stage = Stage::RELEASE;
      }
      break;
    }
    case Stage::SUSTAIN: {
      if (!gate) {
        stage = Stage::RELEASE;
      }
      break;
    }
    case Stage::RELEASE: {
      if (gate && !lastGate) {
        stage = Stage::ATTACK;
      }
      break;
    }
    }
    lastGate = gate;
  }

  inline const sample_t next(sample_t gate) {
    setGate(gate);
    return next();
  }

  inline const sample_t next() {
    sample_t out = nextOutput;
    switch (stage) {
    case Stage::ATTACK: {
      nextOutput += attack_increment;
      if (nextOutput > 1) {
        nextOutput = 1;
        stage = Stage::SUSTAIN;
      }
      break;
    }
    case Stage::SUSTAIN: {
      out = 1;
      break;
    }
    case Stage::RELEASE: {
      nextOutput -= decay_increment;
      if (nextOutput < 0) {
        nextOutput = out = 0;
        stage = Stage::OFF;
      }
      break;
    }
    case Stage::OFF: {
      break;
    }
    }

    return out;
  }
};
template <typename sample_t> struct ADSREnvelope {

  enum Stage { OFF, ATTACK, DECAY, SUSTAIN, RELEASE } stage = OFF;
  sample_t phase = 0;
  sample_t attack_increment = 1.0;
  sample_t decay_increment = 1.0;
  sample_t sustain = 1;
  sample_t release_increment = 1.0;
  sample_t gate = 0;
  void set(const sample_t attack_ms, const sample_t decay_ms,
           const sample_t sustain, const sample_t release_ms,
           const sample_t samplerate) {

    this->sustain = sustain;
    sample_t attack_in_samples = (attack_ms * samplerate) / 1000.0;
    sample_t decay_in_samples = (decay_ms * samplerate) / 1000.0;
    sample_t release_in_samples = (release_ms * samplerate) / 1000.0;
    if (attack_in_samples <= 0) {
      attack_in_samples = 1;
    }
    if (decay_in_samples <= 0) {
      decay_in_samples = 1;
    }
    if (release_in_samples <= 0) {
      release_in_samples = 1;
    }
    attack_increment = 1.0 / attack_in_samples;
    decay_increment = 1.0 / decay_in_samples;
    release_increment = 1.0 / release_in_samples;
  };
  void setGate(const sample_t _gate) {
    if ((_gate > 0) && (gate == 0)) {
      phase = 0;
      stage = Stage::ATTACK;
    }
    gate = _gate;
  };
  inline const sample_t next() {

    sample_t nextValue = 0;
    switch (stage) {
    case Stage::ATTACK: {
      nextValue = phase;
      phase += attack_increment;
      if (phase > 1) {
        phase = 0;
        stage = DECAY;
      }
      break;
    }
    case Stage::DECAY: {
      nextValue = math::lerp<sample_t>(1.0, sustain, phase);
      phase += decay_increment;
      if (phase > 1) {
        phase = 0;
        stage = Stage::SUSTAIN;
      }
      break;
    }
    case Stage::SUSTAIN: {
      nextValue = sustain;
      if (gate == 0) {
        phase = 0;
        stage = Stage::RELEASE;
      }
      break;
    }
    case Stage::RELEASE: {
      nextValue = math::lerp<sample_t>(sustain, 0.0, phase);
      phase += release_increment;
      if (phase > 1) {
        phase = 0;
        stage = Stage::OFF;
      }
      break;
    }
    case Stage::OFF:
      break;
    }
    return nextValue;
  };
  inline const sample_t next(sample_t gate) {
    setGate(gate);
    return next();
  }

  void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      out[i] = next();
    }
  };
  void process(const sample_t *_gate, const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      setGate(_gate[i]);
      out[i] = next();
    }
  };
};

template <typename sample_t>
const sample_t blackman_harris(const sample_t &phase) {
  //     w[n] = a_0 - a_1cos(\frac{2\pi\n}{N}) + a_2cos(\frac{4\pi\n}{N}) -
  //     a_3cos(\frac{6\pi\n}{N})
  // a_0 = 0.35875; a_1 = 0.48829; a_2 = 0.14128; a_3 = 0.01168
  constexpr sample_t FourPi = 2 * math::TwoPi<sample_t>();
  constexpr sample_t SixPi = 3 * math::TwoPi<sample_t>();
  sample_t a_0 = 0.35875;
  sample_t a_1 = 0.48829;
  sample_t a_2 = 0.14128;
  sample_t a_3 = 0.01168;

  return a_0 - a_1 * cos(math::TwoPi<sample_t>() * phase) +
         a_2 * cos(FourPi * phase) - a_3 * cos(SixPi * phase);
}

template <typename sample_t> const sample_t hann(const sample_t &phase) {
  // w[n] = \frac{1}{2} [1 - cos(\frac{2\pi n}{N})] = sin^2 (\frac{\pi n}{N})

  return 0.5 * (1 - cos(math::TwoPi<sample_t>() * phase));
}

template <typename sample_t, const sample_t (*fn)(const sample_t &)>
void apply_window(const sample_t *block, const size_t BLOCKSIZE,
                  sample_t *out) {
  for (size_t idx = 0; idx < BLOCKSIZE; idx++) {
    out[idx] = block[idx] * fn(sample_t(idx) / sample_t(BLOCKSIZE));
  }
}

template <typename sample_t> struct Slew {
  sample_t target;
  sample_t previous;
  sample_t phaseIncrement;
  sample_t phase = 0;
  Slew() {}
  Slew(sample_t v) {
    target = v;
    previous = v;
    phase = 1;
  }
  void set(sample_t _target, sample_t slewTimeMillis, sample_t samplerate) {
    previous = target;
    target = _target;
    sample_t slewTimeSamples = (slewTimeMillis / 1000.0) * samplerate;
    phaseIncrement = (target - previous) / slewTimeSamples;
    phase = 0;
  }
  void process(const size_t blocksize, sample_t *out) {
    for (size_t i = 0; i < blocksize; i++) {
      if (phase < 1) {
        out[i] = math::lerp(previous, target, phase);
        phase += phaseIncrement;
      } else {
        out[i] = target;
      }
    }
  }
};

} // namespace control

namespace spacialization {
template <typename sample_t, size_t TABLE_SIZE = 0> struct Pan {
  sample_t position;
  void setPosition(const sample_t position) { this->position = position; };
  inline void next(const sample_t in, sample_t &outL, sample_t &outR) {

    const sample_t powerLeft = 0.25 * position;
    const sample_t powerRight = powerLeft + 0.75;
    outL =
        in * oscillator::CosineTable<sample_t, TABLE_SIZE>::lookup(powerLeft);
    outR =
        in * oscillator::CosineTable<sample_t, TABLE_SIZE>::lookup(powerRight);
  }
  void process(const sample_t *in, const size_t blocksize, sample_t *outLeft,
               sample_t *outRight) {

    for (size_t i = 0; i < blocksize; i++) {
      next(in[i], outLeft[i], outRight[i]);
    }
  };
};
} // namespace spacialization

namespace effects {
using algae::dsp::filter::InterpolatedDelay;
using algae::dsp::math::lerp;
template <typename sample_t, size_t MAX_DELAY_SAMPS,
          const sample_t (*interp)(const sample_t, const sample_t,
                                   const sample_t) = lerp<sample_t>,
          typename LFO = algae::dsp::oscillator::TriangleLFO<sample_t>>
struct ModulatedDelay {
  InterpolatedDelay<sample_t, MAX_DELAY_SAMPS, interp> delay;
  sample_t delayTimeSamples = MAX_DELAY_SAMPS / 2.0;
  sample_t modDepth = 10;
  LFO lfo;
  ModulatedDelay() { lfo.setFrequency(1, 48000); }
  void setModFrequency(sample_t freq, sample_t samplerate) {
    lfo.setFrequency(freq, samplerate);
  }
  void setDelayTimeMillis(sample_t delayTimeMillis, sample_t samplerate) {
    delayTimeSamples = (delayTimeMillis * samplerate) / 1000.0;
  }
  inline const sample_t tap(int pos) { return delay.tap(pos); }
  const sample_t next(const sample_t in) {
    delay.delayTimeSamples = delayTimeSamples + lfo.next() * modDepth;
    return delay.next(in);
  }
  void process(const sample_t *in, const size_t blocksize, sample_t *out) {
    sample_t lfoSignal[blocksize];
    lfo.process(blocksize, lfoSignal);
    algae::dsp::block::mult(lfoSignal, modDepth, blocksize, lfoSignal);
    for (size_t i = 0; i < blocksize; i++) {
      delay.delayTimeSamples = delayTimeSamples + lfoSignal[i];
      out[i] = delay.next(in[i]);
    }
  }
};

template <typename sample_t, size_t MAX_DELAY_SAMPS,
          const sample_t (*interp)(const sample_t, const sample_t,
                                   const sample_t) = lerp<sample_t>,
          typename LFO = algae::dsp::oscillator::TriangleLFO<sample_t>>
struct ModulatedAllpassFilter
    : _Filter<sample_t,
              ModulatedAllpassFilter<sample_t, MAX_DELAY_SAMPS, interp, LFO>> {
  ModulatedDelay<sample_t, MAX_DELAY_SAMPS, interp, LFO> delay;
  sample_t g = 0;
  sample_t d1 = 0;
  sample_t delayTimeSamples = MAX_DELAY_SAMPS / 2.0;
  sample_t modDepth = 10;
  void setModFrequency(sample_t freq, sample_t samplerate) {
    delay.setModFrequency(freq, samplerate);
  }
  void setDelayTimeMillis(sample_t delayTimeMillis, sample_t samplerate) {
    delayTimeSamples = (delayTimeMillis * samplerate) / 1000.0;
  }
  inline const sample_t tap(int pos) { return delay.tap(pos); }
  inline const sample_t next(const sample_t in) {
    delay.delayTimeSamples = delayTimeSamples;
    delay.modDepth = modDepth;
    sample_t inSum = (in + d1 * g);
    d1 = delay.next(inSum);
    sample_t out = -g * inSum + d1;
    return out;
  }
};

} // namespace effects

} // namespace algae::dsp

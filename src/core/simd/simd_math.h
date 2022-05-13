#pragma once

#ifdef __GNUC__
// #include <immintrin.h>
#include <stdio.h>
#include <stdint.h>

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE 
#endif
#include <math.h>
extern "C" {
#if SSE_INSTR_SET > 7  
// #include <immintrin.h>
// #include <stdint.h>
/* AVX2                                                                                    */
__m256d _ZGVdN4v_cos(__m256d x);               /*  _mm256_cos_pd(x)                        */
__m256d _ZGVdN4v_exp(__m256d x);               /*  _mm256_exp_pd(x)                        */ 
__m256d _ZGVdN4v_log(__m256d x);               /*  _mm256_log_pd(x)                        */
__m256d _ZGVdN4v_sin(__m256d x);               /*  _mm256_sin_pd(x)                        */ 
__m256d _ZGVdN4vv_pow(__m256d x, __m256d y);   /*  _mm256_pow_pd(x, y)                     */
void    _ZGVdN4vvv_sincos(__m256d x, __m256i ptrs, __m256i ptrc);    /* _mm256_sincos_pd   */
static inline __m256d _mm256_cos_pd(__m256d x){
    return _ZGVdN4v_cos(x);
}

static inline __m256d _mm256_exp_pd(__m256d x){
    return _ZGVdN4v_exp(x);
}

static inline __m256d _mm256_log_pd(__m256d x){
    return _ZGVdN4v_log(x);
}

static inline __m256d _mm256_sin_pd(__m256d x){
    return _ZGVdN4v_sin(x);
}

static inline __m256d _mm256_pow_pd(__m256d x, __m256d y){
    return _ZGVdN4vv_pow(x, y);
}

// static void _mm256_sincos_pd(__m256d x, __m256i ptrs, __m256i ptrc){
//     _ZGVdN4vvv_sincos(x, ptrs, ptrc);
// }

/* AVX2                                                                                    */
__m256  _ZGVdN8v_cosf(__m256 x);               /* _mm256_cos_ps(x)                         */
__m256  _ZGVdN8v_expf(__m256 x);               /* _mm256_exp_ps(x)                         */ 
__m256  _ZGVdN8v_logf(__m256 x);               /* _mm256_log_ps(x)                         */
__m256  _ZGVdN8v_sinf(__m256 x);               /* _mm256_sin_ps(x)                         */ 
__m256  _ZGVdN8vv_powf(__m256 x, __m256 y);    /* _mm256_pow_ps(x, y)                      */
void    _ZGVdN8vvv_sincosf(__m256 x, __m256i ptrs_lo, __m256i ptrs_hi, __m256i ptrc_lo, __m256i ptrc_hi);      /* _mm256_sincos_ps   */

static inline __m256 _mm256_cos_ps(__m256 x){
    return _ZGVdN8v_cosf(x);
}

static inline __m256 _mm256_exp_ps(__m256 x){
    return _ZGVdN8v_expf(x);
}

static inline __m256 _mm256_log_ps(__m256 x){
    return _ZGVdN8v_logf(x);
}

static inline __m256 _mm256_sin_ps(__m256 x){
    return _ZGVdN8v_sinf(x);
}

static inline __m256 _mm256_pow_ps(__m256 x, __m256 y){
    return _ZGVdN8vv_powf(x, y);
}

// static void _mm256_sincos_ps(__m256 x, __m256i ptrs_lo, __m256i ptrs_hi, __m256i ptrc_lo, __m256i ptrc_hi){
//     _ZGVdN8vvv_sincosf(x, ptrs_lo, ptrs_hi, ptrc_lo, ptrc_hi);
// }
#elif SSE_INSTR_SET == 7
/* AVX                                                                                     */
__m256d _ZGVcN4v_cos(__m256d x);               /*  _mm256_cos_pd(x)                        */
__m256d _ZGVcN4v_exp(__m256d x);               /*  _mm256_exp_pd(x)                        */ 
__m256d _ZGVcN4v_log(__m256d x);               /*  _mm256_log_pd(x)                        */
__m256d _ZGVcN4v_sin(__m256d x);               /*  _mm256_sin_pd(x)                        */ 
__m256d _ZGVcN4vv_pow(__m256d x, __m256d y);   /*  _mm256_pow_pd(x, y)                     */ 
void    _ZGVcN4vvv_sincos(__m256d x, __m256i ptrs, __m256i ptrc);    /* _mm256_sincos_pd   */

static inline __m256d _mm256_cos_pd(__m256d x){
    return _ZGVcN4v_cos(x);
}

static inline __m256d _mm256_exp_pd(__m256d x){
    return _ZGVcN4v_exp(x);
}

static inline __m256d _mm256_log_pd(__m256d x){
    return _ZGVcN4v_log(x);
}

static inline __m256d _mm256_sin_pd(__m256d x){
    return _ZGVcN4v_sin(x);
}

static inline __m256d _mm256_pow_pd(__m256d x, __m256d y){
    return _ZGVcN4vv_pow(x, y);
}

static void _mm256_sincos_pd(__m256d x, __m256i ptrs, __m256i ptrc){
    _ZGVcN4vvv_sincos(x, ptrs, ptrc);
}

/* AVX                                                                                     */
__m256  _ZGVcN8v_cosf(__m256 x);               /* _mm256_cos_ps(x)                         */
__m256  _ZGVcN8v_expf(__m256 x);               /* _mm256_exp_ps(x)                         */ 
__m256  _ZGVcN8v_logf(__m256 x);               /* _mm256_log_ps(x)                         */
__m256  _ZGVcN8v_sinf(__m256 x);               /* _mm256_sin_ps(x)                         */ 
__m256  _ZGVcN8vv_powf(__m256 x, __m256 y);    /* _mm256_pow_ps(x, y)                      */ 
void    _ZGVcN8vvv_sincosf(__m256 x, __m256i ptrs_lo, __m256i ptrs_hi, __m256i ptrc_lo, __m256i ptrc_hi);      /* _mm256_sincos_ps   */

static inline __m256 _mm256_cos_ps(__m256 x){
    return _ZGVcN8v_cosf(x);
}

static inline __m256 _mm256_exp_ps(__m256 x){
    return _ZGVcN8v_expf(x);
}

static inline __m256 _mm256_log_ps(__m256 x){
    return _ZGVcN8v_logf(x);
}

static inline __m256 _mm256_sin_ps(__m256 x){
    return _ZGVcN8v_sinf(x);
}

static inline __m256 _mm256_pow_ps(__m256 x, __m256 y){
    return _ZGVcN8vv_powf(x, y);
}

// static void _mm256_sincos_ps(__m256 x, __m256i ptrs_lo, __m256i ptrs_hi, __m256i ptrc_lo, __m256i ptrc_hi){
//     _ZGVcN8vvv_sincosf(x, ptrs_lo, ptrs_hi, ptrc_lo, ptrc_hi);
// }
#endif

// #if SSE_INSTR_SET >= 6

/* SSE4                                                                                    */
__m128d _ZGVbN2v_cos(__m128d x);               /*  _mm_cos_pd(x)                           */
__m128d _ZGVbN2v_exp(__m128d x);               /*  _mm_exp_pd(x)                           */ 
__m128d _ZGVbN2v_log(__m128d x);               /*  _mm_log_pd(x)                           */
__m128d _ZGVbN2v_sin(__m128d x);               /*  _mm_sin_pd(x)                           */ 
__m128d _ZGVbN2vv_pow(__m128d x, __m128d y);   /*  _mm_pow_pd(x, y)                        */
void    _ZGVbN2vvv_sincos(__m128d x, __m128i ptrs, __m128i ptrc);    /* _mm_sincos_pd      */

static inline __m128d _mm_cos_pd(__m128d x){
    return _ZGVbN2v_cos(x);
}

static inline __m128d _mm_exp_pd(__m128d x){
    return _ZGVbN2v_exp(x);
}

static inline __m128d _mm_log_pd(__m128d x){
    return _ZGVbN2v_log(x);
}

static inline __m128d _mm_sin_pd(__m128d x){
    return _ZGVbN2v_sin(x);
}

static inline __m128d _mm_pow_pd(__m128d x, __m128d y){
    return _ZGVbN2vv_pow(x, y);
}

// static void _mm_sincos_pd(__m128d x, __m128i ptrs, __m128i ptrc){
//     _ZGVbN2vvv_sincos(x, ptrs, ptrc);
// }



/* Single precision Libmvec                       Intel SVML function                      */

/* SSE4                                                                                    */
__m128  _ZGVbN4v_cosf(__m128 x);               /* _mm_cos_ps(x)                            */
__m128  _ZGVbN4v_expf(__m128 x);               /* _mm_exp_ps(x)                            */ 
__m128  _ZGVbN4v_logf(__m128 x);               /* _mm_log_ps(x)                            */
__m128  _ZGVbN4v_sinf(__m128 x);               /* _mm_sin_ps(x)                            */ 
__m128  _ZGVbN4vv_powf(__m128 x, __m128 y);    /* _mm_pow_ps(x, y) */ 
void    _ZGVbN4vvv_sincosf(__m128 x, __m128i ptrs_lo, __m128i ptrs_hi, __m128i ptrc_lo, __m128i ptrc_hi);      /* _mm_sincos_ps   */

static inline __m128 _mm_cos_ps(__m128 x){
    return _ZGVbN4v_cosf(x);
}

static inline __m128 _mm_exp_ps(__m128 x){
    return _ZGVbN4v_expf(x);
}

static inline __m128 _mm_log_ps(__m128 x){
    return _ZGVbN4v_logf(x);
}

static inline __m128 _mm_sin_ps(__m128 x){
    return _ZGVbN4v_sinf(x);
}

static inline __m128 _mm_pow_ps(__m128 x, __m128 y){
    return _ZGVbN4vv_powf(x, y);
}

// static void _mm_sincos_ps(__m128 x, __m128i ptrs_lo, __m128i ptrs_hi, __m128i ptrc_lo, __m128i ptrc_hi){
//     _ZGVbN4vvv_sincosf(x, ptrs_lo, ptrs_hi, ptrc_lo, ptrc_hi);
// }
// #endif
}
#endif
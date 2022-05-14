#include <gtest/gtest.h>
#include "../../src/core/simd.h"
#include <cmath>

using algae::dsp::core::simd::simd_traits;
using algae::dsp::core::simd::simd_vector_traits;
using algae::dsp::core::simd::load_a;
using algae::dsp::core::simd::load_u;

typedef typename simd_traits<float>::type float_vec;
constexpr size_t float_vec_size =  simd_traits<float>::size;
typedef typename simd_traits<double>::type double_vec;
constexpr size_t double_vec_size = simd_traits<double>::size;

TEST(SIMD_Test, vector_load_u_float) {
    float a[float_vec_size];
    for(size_t idx=0; idx<float_vec_size; idx++){
        a[idx] = float(idx);
    }
    float_vec vec = load_u(a);
    float b[float_vec_size];
    store_u(b, vec);
    for(size_t idx=0; idx<float_vec_size; idx++){
        EXPECT_EQ(a[idx], b[idx]);
    }
}

TEST(SIMD_Test, vector_load_a_float) {
    alignas(16) float a[float_vec_size];
    for(size_t idx=0; idx<float_vec_size; idx++){
        a[idx] = float(idx);
    }
    float_vec vec = load_a(a);
    alignas(16) float b[float_vec_size];
    store_a(b, vec);
    for(size_t idx=0; idx<float_vec_size; idx++){
        EXPECT_EQ(a[idx], b[idx]);
    }

}

TEST(SIMD_Test, vector_load_u_double) {
    double a[double_vec_size];
    for(size_t idx=0; idx<double_vec_size; idx++){
        a[idx] = double(idx);
    }
    double_vec vec = load_u(a);
    double b[double_vec_size];
    store_u(b, vec);
    for(size_t idx=0; idx<double_vec_size; idx++){
        EXPECT_EQ(a[idx], b[idx]);
    }

}

TEST(SIMD_Test, vector_load_a_double) {
    alignas(16) double a[double_vec_size];
    for(size_t idx=0; idx<double_vec_size; idx++){
        a[idx] = double(idx);
    }
    double_vec vec = load_a(a);
    alignas(16) double b[double_vec_size];
    store_a(b, vec);
    for(size_t idx=0; idx<double_vec_size; idx++){
        EXPECT_EQ(a[idx], b[idx]);
    }

}

TEST(SIMD_Test, vector_index_float) {
    float a[float_vec_size];
    for(size_t idx=0; idx<float_vec_size; idx++){
        a[idx] = float(idx);
    }
    float_vec vec = load_a(a);
    for(size_t idx=0; idx<float_vec_size; idx++){
        EXPECT_EQ(a[idx],vec[idx]);
    }

}
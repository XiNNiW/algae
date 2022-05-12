#pragma once

#include "simd_base.h"


template <>
struct simd_traits<float>
{
    typedef vector8f type;
    static const size_t size = 8;
};

template <>
struct simd_traits<double>
{
    typedef vector4d type;
    static const size_t size = 4;
};

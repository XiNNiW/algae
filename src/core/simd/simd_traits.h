#pragma once

template <class T, size_t S=1>
struct simd_traits
{
    typedef T type;
    static const size_t size = S;
};
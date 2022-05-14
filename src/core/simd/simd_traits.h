#pragma once
namespace algae::dsp::core::simd{

template <class T>
struct simd_traits
{
    typedef T type;
    static const size_t size = 1;
};

template <class X>
struct simd_vector_traits{
    typedef X value_type;
    typedef bool vector_bool;
};

}
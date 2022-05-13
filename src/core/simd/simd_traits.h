#pragma once
namespace algae::dsp::core::simd{

template <class T>
struct simd_traits
{
    typedef T type;
    static const size_t size = 1;
};

}
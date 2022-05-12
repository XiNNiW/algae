#pragma once

#include "simd_base.h"
class vector4f;
class vector2d;

template <>
struct simd_traits<float>
{
    typedef vector4f type;
    static const size_t size = 4;
};

template <>
struct simd_traits<double>
{
    typedef vector2d type;
    static const size_t size = 2;
};

template <>
struct simd_vector_traits<vector4f>
{
    typedef float value_type;
    // typedef simd_vector4fb vector_bool;
};


class vector4f : public simd_vector<vector4f>
{
public:

    inline vector4f() {}
    inline vector4f(float f) : m_value(_mm_set1_ps(f)) {}
    inline vector4f(float f0, float f1, float f2, float f3) : m_value(_mm_setr_ps(f0,f1,f2,f3)) {}
    inline vector4f(const __m128& rhs) : m_value(rhs) {}

    inline vector4f& operator=(const __m128& rhs)
    {
        m_value = rhs;
        return *this;
    }

    inline operator __m128() const { return m_value; }

    inline vector4f& load_a(const float* src)
    {
        m_value = _mm_load_ps(src);
        return *this;
    }

    inline vector4f& load_u(const float* src)
    {
        m_value = _mm_loadu_ps(src);
        return *this;
    }

    inline void store_a(float* dst) const
    {
        _mm_store_ps(dst,m_value);
    }

    inline void store_u(float* dst) const
    {
        _mm_storeu_ps(dst,m_value);
    }

    // No more operator+= since it is implemented in the base class

private:

    __m128 m_value;
};

inline vector4f operator+(const vector4f& lhs, const vector4f& rhs)
{
    return _mm_add_ps(lhs,rhs);
}

inline vector4f operator-(const vector4f& lhs, const vector4f& rhs)
{
    return _mm_sub_ps(lhs,rhs);
}

inline vector4f operator*(const vector4f& lhs, const vector4f& rhs)
{
    return _mm_mul_ps(lhs,rhs);
}

inline vector4f operator/(const vector4f& lhs, const vector4f& rhs)
{
    return _mm_div_ps(lhs,rhs);
}

// inline vector4fb operator==(const vector4f& lhs, const vector4f& rhs)
// {
//     return _mm_cmpeq_ps(lhs,rhs);
// }

// inline vector4fb operator!=(const vector4f& lhs, const vector4f& rhs)
// {
//     return _mm_cmpneq_ps(lhs,rhs);
// }

// inline vector4fb operator<(const vector4f& lhs, const vector4f& rhs)
// {
//     return _mm_cmplt_ps(lhs,rhs);
// }

// inline vector4fb operator<=(const vector4f& lhs, const vector4f& rhs)
// {
//     return _mm_cmple_ps(lhs,rhs);
// }

// class vector2d : public simd_vector<vector4f>
// {
// public:

//     inline vector2d() {}
//     inline vector2d(double f) : m_value(_mm_set1_pd(f)) {}
//     inline vector2d(double d0, double d2) : m_value(_mm_setr_pd(d0,d1)) {}
//     inline vector2d(const __m128& rhs) : m_value(rhs) {}

//     inline vector2d& operator=(const __m128& rhs)
//     {
//         m_value = rhs;
//         return *this;
//     }

//     inline operator __m128() const { return m_value; }

//     inline vector2d& load_a(const double* src)
//     {
//         m_value = _mm_load_pd(src);
//         return *this;
//     }

//     inline vector2d& load_u(const double* src)
//     {
//         m_value = _mm_loadu_pd(src);
//         return *this;
//     }

//     inline void store_a(double* dst) const
//     {
//         _mm_store_pd(dst,m_value);
//     }

//     inline void store_u(double* dst) const
//     {
//         _mm_storeu_pd(dst,m_value);
//     }
// };

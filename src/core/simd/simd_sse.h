/* 
The SIMD types implementation is heavily based on ___'s implementation published here: https://johanmabille.github.io/blog/2014/10/09/writing-c-plus-plus-wrappers-for-simd-intrinsics-1/
*/
#pragma once

#include "simd_base.h"
class vector4f;
class vector2d;
class vector4fb;
class vector2db;

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
    typedef vector4fb vector_bool;
};

template <>
struct simd_vector_traits<vector2d>
{
    typedef float value_type;
    typedef vector2db vector_bool;
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

class vector4fb : public simd_vector_bool<vector4fb>
{
public:

    inline vector4fb() {}
    inline vector4fb(bool b) : m_value(_mm_castsi128_ps(_mm_set1_epi32(-(int)b))) {}
    inline vector4fb(bool b0, bool b1, bool b2, bool b3)
    : m_value(_mm_castsi128_ps(_mm_setr_epi32(-(int)b0,-(int)b1,-(int)b2,-(int)b3))) {}

    inline vector4fb(const __m128& rhs) : m_value(rhs) {}

    inline vector4fb& operator=(const __m128& rhs)
    {
        m_value = rhs;
        return *this;
    }

    inline operator __m128() const { return m_value; }

    inline vector4fb& load(const bool* src)
    {
        *this = vector4fb(src[0],src[1],src[2],src[3]);
        return *this;
    }


    inline void store(bool* dst) const
    {
        float dstf[4];
        _mm_store_ps(dstf,m_value);
        dst[0] = dstf[0]!=0;
        dst[1] = dstf[1]!=0;
        dst[2] = dstf[2]!=0;
        dst[3] = dstf[3]!=0;
    }



private:

    __m128 m_value;
};

inline vector4fb operator&(const vector4fb& lhs, const vector4fb& rhs)
{
    return _mm_and_ps(lhs,rhs);
}

inline vector4fb operator|(const vector4fb& lhs, const vector4fb& rhs)
{
    return _mm_or_ps(lhs,rhs);
}

inline vector4fb operator^(const vector4fb& lhs, const vector4fb& rhs)
{
    return _mm_xor_ps(lhs,rhs);
}

inline vector4fb operator~(const vector4fb& rhs)
{
    return _mm_xor_ps(rhs,_mm_castsi128_ps(_mm_set1_epi32(-1)));
}

inline vector4fb operator==(const vector4fb& lhs, const vector4fb& rhs)
{
    return _mm_cmpeq_ps(lhs,rhs);
}

inline vector4fb operator!=(const vector4f& lhs, const vector4fb& rhs)
{
    return _mm_cmpneq_ps(lhs,rhs);
}

inline vector4fb operator==(const vector4f& lhs, const vector4f& rhs)
{
    return _mm_cmpeq_ps(lhs,rhs);
}

inline vector4fb operator!=(const vector4f& lhs, const vector4f& rhs)
{
    return _mm_cmpneq_ps(lhs,rhs);
}

inline vector4fb operator<(const vector4f& lhs, const vector4f& rhs)
{
    return _mm_cmplt_ps(lhs,rhs);
}

inline vector4fb operator<=(const vector4f& lhs, const vector4f& rhs)
{
    return _mm_cmple_ps(lhs,rhs);
}

inline vector4fb operator>(const vector4f& lhs, const vector4f& rhs)
{
    return _mm_cmpgt_ps(lhs,rhs);
}

inline vector4fb operator>=(const vector4f& lhs, const vector4f& rhs)
{
    return _mm_cmpge_ps(lhs,rhs);
}

class vector2d : public simd_vector<vector2d>
{
public:

    inline vector2d() {}
    inline vector2d(double f) : m_value(_mm_set1_pd(f)) {}
    inline vector2d(double d0, double d1) : m_value(_mm_setr_pd(d0,d1)) {}
    inline vector2d(const __m128d& rhs) : m_value(rhs) {}

    inline vector2d& operator=(const __m128d& rhs)
    {
        m_value = rhs;
        return *this;
    }

    inline operator __m128d() const { return m_value; }

    inline vector2d& load_a(const double* src)
    {
        m_value = _mm_load_pd(src);
        return *this;
    }

    inline vector2d& load_u(const double* src)
    {
        m_value = _mm_loadu_pd(src);
        return *this;
    }

    inline void store_a(double* dst) const
    {
        _mm_store_pd(dst,m_value);
    }

    inline void store_u(double* dst) const
    {
        _mm_storeu_pd(dst,m_value);
    }
private:

    __m128d m_value;
};

inline vector2d operator+(const vector2d& lhs, const vector2d& rhs)
{
    return _mm_add_pd(lhs,rhs);
}

inline vector2d operator-(const vector2d& lhs, const vector2d& rhs)
{
    return _mm_sub_pd(lhs,rhs);
}

inline vector2d operator*(const vector2d& lhs, const vector2d& rhs)
{
    return _mm_mul_pd(lhs,rhs);
}

inline vector2d operator/(const vector2d& lhs, const vector2d& rhs)
{
    return _mm_div_pd(lhs,rhs);
}


class vector2db : public simd_vector_bool<vector2db>
{
public:

    inline vector2db() {}
    inline vector2db(bool b) : m_value(_mm_castsi128_pd(_mm_set1_epi64x(-(int)b))) {}
    inline vector2db(bool b0, bool b1)
    : m_value(_mm_castsi128_pd(_mm_setr_epi64(_mm_cvtsi32_si64(-(int)b0),_mm_cvtsi32_si64(-(int)b1)))) {}

    inline vector2db(const __m128d& rhs) : m_value(rhs) {}

    inline vector2db& operator=(const __m128d& rhs)
    {
        m_value = rhs;
        return *this;
    }

    inline operator __m128d() const { return m_value; }

    inline vector2db& load(const bool* src)
    {
        *this = vector2db(src[0],src[1]);
        return *this;
    }


    inline void store(bool* dst) const
    {
        double dstf[2];
        _mm_store_pd(dstf,m_value);
        dst[0] = dstf[0]!=0;
        dst[1] = dstf[1]!=0;
    }



private:

    __m128d m_value;
};

inline vector2db operator&(const vector2db& lhs, const vector2db& rhs)
{
    return _mm_and_pd(lhs,rhs);
}

inline vector2db operator|(const vector2db& lhs, const vector2db& rhs)
{
    return _mm_or_pd(lhs,rhs);
}

inline vector2db operator^(const vector2db& lhs, const vector2db& rhs)
{
    return _mm_xor_pd(lhs,rhs);
}

inline vector2db operator~(const vector2db& rhs)
{
    return _mm_xor_pd(rhs,_mm_castsi128_pd(_mm_set1_epi64x(-1)));
}

inline vector2db operator==(const vector2db& lhs, const vector2db& rhs)
{
    return _mm_cmpeq_pd(lhs,rhs);
}

inline vector2db operator!=(const vector2db& lhs, const vector2db& rhs)
{
    return _mm_cmpneq_pd(lhs,rhs);
}


inline vector2db operator==(const vector2d& lhs, const vector2d& rhs)
{
    return _mm_cmpeq_pd(lhs,rhs);
}

inline vector2db operator!=(const vector2d& lhs, const vector2d& rhs)
{
    return _mm_cmpneq_pd(lhs,rhs);
}

inline vector2db operator<(const vector2d& lhs, const vector2d& rhs)
{
    return _mm_cmplt_pd(lhs,rhs);
}

inline vector2db operator<=(const vector2d& lhs, const vector2d& rhs)
{
    return _mm_cmple_pd(lhs,rhs);
}

inline vector2db operator>(const vector2d& lhs, const vector2d& rhs)
{
    return _mm_cmpgt_pd(lhs,rhs);
}

inline vector2db operator>=(const vector2d& lhs, const vector2d& rhs)
{
    return _mm_cmpge_pd(lhs,rhs);
}

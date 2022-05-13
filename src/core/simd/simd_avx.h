#pragma once

#include "simd_base.h"

namespace algae::dsp::core::simd{


class vector8f;
class vector4d;
class vector8fb;
class vector4db;


template <>
struct simd_vector_traits<vector8f>
{
    typedef float value_type;
    typedef vector8fb vector_bool;
};

template <>
struct simd_vector_traits<vector4d>
{
    typedef double value_type;
    typedef vector4db vector_bool;
};


class vector8f : public simd_vector<vector8f>
{
public:

    inline vector8f() {}
    inline vector8f(float f) : m_value(_mm256_set1_ps(f)) {}
    inline vector8f(float f0, float f1, float f2, float f3,float f4, float f5, float f6, float f7) 
        : m_value(_mm256_setr_ps(f0,f1,f2,f3,f4,f5,f6,f7)) {}
    inline vector8f(const __m256& rhs) : m_value(rhs) {}

    inline vector8f& operator=(const __m256& rhs)
    {
        m_value = rhs;
        return *this;
    }

    inline operator __m256() const { return m_value; }

    inline vector8f& load_a(const float* src)
    {
        m_value = _mm256_load_ps(src);
        return *this;
    }

    inline vector8f& load_u(const float* src)
    {
        m_value = _mm256_loadu_ps(src);
        return *this;
    }

    inline void store_a(float* dst) const
    {
        _mm256_store_ps(dst,m_value);
    }

    inline void store_u(float* dst) const
    {
        _mm256_storeu_ps(dst,m_value);
    }


private:

    __m256 m_value;
};

inline vector8f operator+(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_add_ps(lhs,rhs);
}

inline vector8f operator-(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_sub_ps(lhs,rhs);
}

inline vector8f operator*(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_mul_ps(lhs,rhs);
}

inline vector8f operator/(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_div_ps(lhs,rhs);
}

inline vector8f cos(const vector8f& lhs)
{
    return _mm256_cos_ps(lhs);
}

inline vector8f exp(const vector8f& lhs)
{
    return _mm256_exp_ps(lhs);
}

inline vector8f log(const vector8f& lhs)
{
    return _mm256_log_ps(lhs);
}

inline vector8f sin(const vector8f& lhs)
{
    return _mm256_sin_ps(lhs);
}

inline vector8f pow(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_pow_ps(lhs, rhs);
}

inline vector8f sqrt(const vector8f& lhs)
{
    return _mm256_sqrt_ps(lhs);
}


class vector8fb : public simd_vector_bool<vector8fb>
{
public:

    inline vector8fb() {}
    inline vector8fb(bool b) : m_value(_mm256_castsi256_ps(_mm256_set1_epi32(-(int)b))) {}
    inline vector8fb(bool b0, bool b1, bool b2, bool b3,bool b4, bool b5, bool b6, bool b7)
    : m_value(_mm256_castsi256_ps(_mm256_setr_epi32(-(int)b0,-(int)b1,-(int)b2,-(int)b3,-(int)b4,-(int)b5,-(int)b6,-(int)b7))) {}

    inline vector8fb(const __m256& rhs) : m_value(rhs) {}

    inline vector8fb& operator=(const __m256& rhs)
    {
        m_value = rhs;
        return *this;
    }

    inline operator __m256() const { return m_value; }

    inline vector8fb& load(const bool* src)
    {
        *this = vector8fb(src[0],src[1],src[2],src[3],src[4],src[5],src[6],src[7]);
        return *this;
    }


    inline void store(bool* dst) const
    {
        float dstf[8];
        _mm256_store_ps(dstf,m_value);
        dst[0] = dstf[0]!=0;
        dst[1] = dstf[1]!=0;
        dst[2] = dstf[2]!=0;
        dst[3] = dstf[3]!=0;
        dst[4] = dstf[4]!=0;
        dst[5] = dstf[5]!=0;
        dst[6] = dstf[6]!=0;
        dst[7] = dstf[7]!=0;
    }



private:

    __m256 m_value;
};

inline vector8fb operator&(const vector8fb& lhs, const vector8fb& rhs)
{
    return _mm256_and_ps(lhs,rhs);
}

inline vector8fb operator|(const vector8fb& lhs, const vector8fb& rhs)
{
    return _mm256_or_ps(lhs,rhs);
}

inline vector8fb operator^(const vector8fb& lhs, const vector8fb& rhs)
{
    return _mm256_xor_ps(lhs,rhs);
}

inline vector8fb operator~(const vector8fb& rhs)
{
    return _mm256_xor_ps(rhs,_mm256_castsi256_ps(_mm256_set1_epi32(-1)));
}

inline vector8fb operator==(const vector8fb& lhs, const vector8fb& rhs)
{
    return _mm256_cmp_ps(lhs,rhs,0);
}

inline vector8fb operator!=(const vector8f& lhs, const vector8fb& rhs)
{
    return _mm256_cmp_ps(lhs,rhs,0);
}

inline vector8fb operator==(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_cmp_ps(lhs,rhs,_CMP_EQ_OQ);
}

inline vector8fb operator!=(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_cmp_ps(lhs,rhs,_CMP_NEQ_OQ);
}

inline vector8fb operator<(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_cmp_ps(lhs,rhs,_CMP_LT_OQ);
}

inline vector8fb operator>(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_cmp_ps(lhs,rhs,_CMP_GT_OQ);
}

inline vector8fb operator<=(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_cmp_ps(lhs,rhs,_CMP_LE_OQ);
}

inline vector8fb operator>=(const vector8f& lhs, const vector8f& rhs)
{
    return _mm256_cmp_ps(lhs,rhs,_CMP_GE_OQ);
}

inline vector8f select(const vector8fb& cond, const vector8f& a, const vector8f& b)
{
#if SSE_INSTR_SET >= 5 // SSE 4.1
    return _mm256_blendv_ps(b,a,cond);
#else
    return _mm256_or_ps(_mm256_and_ps(cond,a),_mm256_andnot_ps(cond,b));
#endif
}

//--------------------

class vector4d : public simd_vector<vector4d>
{
public:

    inline vector4d() {}
    inline vector4d(double f) : m_value(_mm256_set1_pd(f)) {}
    inline vector4d(double f0, double f1, double f2, double f3) 
        : m_value(_mm256_setr_pd(f0,f1,f2,f3)) {}
    inline vector4d(const __m256d& rhs) : m_value(rhs) {}

    inline vector4d& operator=(const __m256d& rhs)
    {
        m_value = rhs;
        return *this;
    }

    inline operator __m256d() const { return m_value; }

    inline vector4d& load_a(const double* src)
    {
        m_value = _mm256_load_pd(src);
        return *this;
    }

    inline vector4d& load_u(const double* src)
    {
        m_value = _mm256_loadu_pd(src);
        return *this;
    }

    inline void store_a(double* dst) const
    {
        _mm256_store_pd(dst,m_value);
    }

    inline void store_u(double* dst) const
    {
        _mm256_storeu_pd(dst,m_value);
    }


private:

    __m256d m_value;
};

inline vector4d operator+(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_add_pd(lhs,rhs);
}

inline vector4d operator-(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_sub_pd(lhs,rhs);
}

inline vector4d operator*(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_mul_pd(lhs,rhs);
}

inline vector4d operator/(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_div_pd(lhs,rhs);
}


inline vector4d cos(const vector4d& lhs)
{
    return _mm256_cos_pd(lhs);
}

inline vector4d exp(const vector4d& lhs)
{
    return _mm256_exp_pd(lhs);
}

inline vector4d log(const vector4d& lhs)
{
    return _mm256_log_pd(lhs);
}

inline vector4d sin(const vector4d& lhs)
{
    return _mm256_sin_pd(lhs);
}

inline vector4d pow(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_pow_pd(lhs, rhs);
}

inline vector4d sqrt(const vector4d& lhs)
{
    return _mm256_sqrt_pd(lhs);
}



class vector4db : public simd_vector_bool<vector4db>
{
public:

    inline vector4db() {}
    inline vector4db(bool b) : m_value(_mm256_castsi256_pd(_mm256_set1_epi64x(-(int)b))) {}
    inline vector4db(bool b0, bool b1, bool b2, bool b3)
    : m_value(_mm256_castsi256_pd(_mm256_setr_epi64x(-(int)b0,-(int)b1,-(int)b2,-(int)b3))) {}

    inline vector4db(const __m256d& rhs) : m_value(rhs) {}

    inline vector4db& operator=(const __m256d& rhs)
    {
        m_value = rhs;
        return *this;
    }

    inline operator __m256d() const { return m_value; }

    inline vector4db& load(const bool* src)
    {
        *this = vector4db(src[0],src[1],src[2],src[3]);
        return *this;
    }


    inline void store(bool* dst) const
    {
        double dstf[4];
        _mm256_store_pd(dstf,m_value);
        dst[0] = dstf[0]!=0;
        dst[1] = dstf[1]!=0;
        dst[2] = dstf[2]!=0;
        dst[3] = dstf[3]!=0;
    }



private:

    __m256d m_value;
};

inline vector4db operator&(const vector4db& lhs, const vector4db& rhs)
{
    return _mm256_and_pd(lhs,rhs);
}

inline vector4db operator|(const vector4db& lhs, const vector4db& rhs)
{
    return _mm256_or_pd(lhs,rhs);
}

inline vector4db operator^(const vector4db& lhs, const vector4db& rhs)
{
    return _mm256_xor_pd(lhs,rhs);
}

inline vector4db operator~(const vector4db& rhs)
{
    return _mm256_xor_pd(rhs,_mm256_castsi256_pd(_mm256_set1_epi64x(-1)));
}

inline vector4db operator==(const vector4db& lhs, const vector4db& rhs)
{
    return _mm256_cmp_pd(lhs,rhs,0);
}

inline vector4db operator!=(const vector4db& lhs, const vector4db& rhs)
{
    return _mm256_cmp_pd(lhs,rhs,0);
}

inline vector4db operator==(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_cmp_pd(lhs,rhs,_CMP_EQ_OQ);
}

inline vector4db operator!=(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_cmp_pd(lhs,rhs,_CMP_NEQ_OQ);
}

inline vector4db operator<(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_cmp_pd(lhs,rhs,_CMP_LT_OQ);
}

inline vector4db operator>(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_cmp_pd(lhs,rhs,_CMP_GT_OQ);
}

inline vector4db operator<=(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_cmp_pd(lhs,rhs,_CMP_LE_OQ);
}

inline vector4db operator>=(const vector4d& lhs, const vector4d& rhs)
{
    return _mm256_cmp_pd(lhs,rhs,_CMP_GE_OQ);
}

inline vector4d select(const vector4db& cond, const vector4d& a, const vector4d& b)
{
#if SSE_INSTR_SET >= 5 // SSE 4.1
    return _mm256_blendv_pd(b,a,cond);
#else
    return _mm256_or_pd(_mm256_and_pd(cond,a),_mm256_andnot_pd(cond,b));
#endif
}

}
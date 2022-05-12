#pragma once
#include "simd_traits.h"

template <class X>
struct simd_vector_traits;

template <class X>
class simd_vector_bool
{
public:

    inline X& operator()() { return *static_cast<X*>(this); }
    inline const X& operator()() const { return *static_cast<const X*>(this); }

    inline X& operator&=(const X& rhs)
    {
        (*this) = (*this) && rhs;
        return (*this)();
    }

    inline X& operator|(const X& rhs)
    {
        (*this)() = (*this) || rhs;
        return (*this)();
    }

    inline X& operator^=(const X& rhs)
    {
        (*this)() = (*this)() ^ rhs;
        return (*this)();
    }

protected:

    inline simd_vector_bool() {}
    inline ~simd_vector_bool() {}

    inline simd_vector_bool(const simd_vector_bool&) {}
    inline simd_vector_bool& operator=(const simd_vector_bool&) { return *this; }
};

template <class X>
inline X operator&&(const simd_vector_bool<X>& lhs, const simd_vector_bool<X>& rhs)
{
    return lhs() & rhs();
}

template <class X>
inline X operator&&(const simd_vector_bool<X>& lhs, bool rhs)
{
    return lhs() & rhs;
}

template <class X>
inline X operator||(bool lhs, const simd_vector_bool<X>& rhs)
{
    return lhs & rhs();
}

// Similar for operator|| overloads
// ...

template <class X>
inline X operator!(const simd_vector_bool<X>& rhs)
{
    return rhs() == 0;
}

template <class X>
class simd_vector
{
public:

    typedef typename simd_vector_traits<X>::value_type value_type;

    // downcast operators so we can call methods in the inheriting classes
    inline X& operator()() { return *static_cast<X*>(this); }
    inline const X& operator()() const { return *static_cast<const X*>(this); }

    // Additional assignment operators
    inline X& operator+=(const X& rhs)
    {
        (*this)() = (*this)() + rhs;
        return (*this)();
    }

    inline X& operator+=(const value_type& rhs)
    {
        (*this)() = (*this)() + X(rhs);
        return (*this)();
    }

    // Same for operator-=, operator*=, operator/= ...
    // ...

    // Increment operators
    inline X operator++(int)
    {
        X tmp = (*this)();
        (*this) += value_type(1);
        return tmp;
    }

    inline X& operator++()
    {
        (*this)() += value_type(1);
        return (*this)();
    }

    // Similar decrement operators
    // ...

protected:

    // Ensure only inheriting classes can instantiate / copy / assign simd_vector.
    // Avoids incomplete copy / assignment from client code.
    inline simd_vector() {}
    inline ~simd_vector() {}

    inline simd_vector(const simd_vector&) {}
    inline simd_vector& operator=(const  simd_vector&) { return *this; }
};

template <class X>
inline simd_vector<X> operator+(const simd_vector<X>& lhs,
                                const typename simd_vector_traits<X>::value_type& rhs)
{
    return lhs() + X(rhs);
}

template <class X>
inline simd_vector<X> operator+(const typename simd_vector_traits<X>::value_type& lhs,
                                const simd_vector<X>& rhs)
{
    return X(lhs) + rhs();
}

template <class X>
inline simd_vector<X> operator-(const simd_vector<X>& lhs,
                                const typename simd_vector_traits<X>::value_type& rhs)
{
    return lhs() - X(rhs);
}

template <class X>
inline simd_vector<X> operator-(const typename simd_vector_traits<X>::value_type& lhs,
                                const simd_vector<X>& rhs)
{
    return X(lhs) - rhs();
}

template <class X>
inline simd_vector<X> operator*(const simd_vector<X>& lhs,
                                const typename simd_vector_traits<X>::value_type& rhs)
{
    return lhs() * X(rhs);
}

template <class X>
inline simd_vector<X> operator*(const typename simd_vector_traits<X>::value_type& lhs,
                                const simd_vector<X>& rhs)
{
    return X(lhs) * rhs();
}

template <class X>
inline simd_vector<X> operator/(const simd_vector<X>& lhs,
                                const typename simd_vector_traits<X>::value_type& rhs)
{
    return lhs() / X(rhs);
}

template <class X>
inline simd_vector<X> operator/(const typename simd_vector_traits<X>::value_type& lhs,
                                const simd_vector<X>& rhs)
{
    return X(lhs) / rhs();
}


// Same for operator-, operator*, operator/
// ...

// template <class X>
// inline typename simd_vector_traits<X>::vector_bool
// operator>(const simd_vector<X>& lhs, const simd_vector<X>& rhs)
// {
//     return rhs() <= lhs();
// }

// template <class X>
// inline typename simd_vector_traits<X>::vector_bool
// operator>=(const simd_vector<X>& lhs, const simd_vector<X>& rhs)
// {
//     return rhs() < lhs();
// }
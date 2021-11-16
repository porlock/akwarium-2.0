#pragma once
#include <stdint.h>


template<typename T, T Denominator>
class FixedPoint
{
public:

    FixedPoint() : m_value(0)
    {
    }

    FixedPoint(float f)
    {
        m_value = T(f * Denominator);
    }

    FixedPoint(const FixedPoint& f) = default;
    FixedPoint(FixedPoint&& f) = default;
    FixedPoint& operator=(const FixedPoint& b) = default;
    FixedPoint& operator=(FixedPoint&& b) = default;

    FixedPoint& operator+=(const FixedPoint& b) { m_value += b.m_value; return *this; }
    const FixedPoint operator+(const FixedPoint& b) const { FixedPoint result = *this; result += b; return result; }

    FixedPoint& operator-=(const FixedPoint& b) { m_value -= b.m_value; return *this; }
    const FixedPoint operator-(const FixedPoint& b) const { FixedPoint result = *this; result -= b; return result; }
    const FixedPoint operator-() const { FixedPoint negative(*this); negative.m_value *= -1; return negative; }

    FixedPoint& operator*=(const FixedPoint& b) { m_value *= b.m_value; return *this; }
    const FixedPoint operator*(const FixedPoint& b) const { FixedPoint result = *this; result *= b; return result; }

    FixedPoint& operator/=(const FixedPoint& b) { m_value /= b.m_value; return *this; }
    const FixedPoint operator/(const FixedPoint& b) const { FixedPoint result = *this; result /= b; return result; }

    FixedPoint& operator%=(const FixedPoint& b) { m_value %= b.m_value; return *this; }
    const FixedPoint operator%(const FixedPoint& b) const { FixedPoint result = *this; result %= b; return result; }

 
    bool operator == (const FixedPoint& b) const { return m_value == b.m_value; }
    bool operator != (const FixedPoint& b) const { return m_value != b.m_value; }
    bool operator < (const FixedPoint& b) const { return m_value < b.m_value; }
    bool operator > (const FixedPoint& b) const { return m_value > b.m_value; }
    bool operator <= (const FixedPoint& b) const { return m_value <= b.m_value; }
    bool operator >= (const FixedPoint& b) const { return m_value >= b.m_value; }

    T integer() const
    {
        return m_value / Denominator;
    }

    T fraction() const
    {
        const T frac = (m_value % Denominator);
        return frac < 0 ? frac * -1 : frac;
    }

    float as_float() const
    {
        return float(m_value) / Denominator;
    }

protected:

    T m_value;
};

typedef FixedPoint<int8_t, 10>      F8x1;
typedef FixedPoint<uint8_t, 10>     UF8x1;

typedef FixedPoint<int16_t, 10>     F16x1;
typedef FixedPoint<uint16_t, 10>    UF16x1;
typedef FixedPoint<int16_t, 100>    F16x2;
typedef FixedPoint<uint16_t, 100>   UF16x2;
typedef FixedPoint<int16_t, 1000>    F16x3;
typedef FixedPoint<uint16_t, 1000>   UF16x3;

typedef FixedPoint<int32_t, 10>     F32x1;
typedef FixedPoint<uint32_t, 10>    UF32x1;
typedef FixedPoint<int32_t, 100>    F32x2;
typedef FixedPoint<uint32_t, 100>   UF32x2;
typedef FixedPoint<int32_t, 1000>   F32x3;
typedef FixedPoint<uint32_t, 1000>  UF32x3;
typedef FixedPoint<int32_t, 10000>  F32x4;
typedef FixedPoint<uint32_t, 10000> UF32x4;
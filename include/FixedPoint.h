#pragma once
#include <stdint.h>

template<typename T, uint32_t MUL>
class FixedPoint
{
public:

    FixedPoint(float f)
    {
        m_value = T(f * MUL);
    }

    FixedPoint(uint32_t integer, uint32_t fraction)
    {
        m_value = T(integer * MUL) + T(fraction);
    }

    FixedPoint(const FixedPoint& f) = default;
    FixedPoint(FixedPoint&& f) = default;
    FixedPoint& operator=(const FixedPoint& b) = default;
    FixedPoint& operator=(FixedPoint&& b) = default;

    FixedPoint& operator+=(const FixedPoint& b) { m_value += b.m_value; return *this; }
    const FixedPoint operator+(const FixedPoint& b) const { FixedPoint result = *this; result += b; return result; }

    FixedPoint& operator-=(const FixedPoint& b) { m_value -= b.m_value; return *this; }
    const FixedPoint operator-(const FixedPoint& b) const { FixedPoint result = *this; result -= b; return result; }

    FixedPoint& operator*=(const FixedPoint& b) { m_value *= b.m_value; return *this; }
    const FixedPoint operator*(const FixedPoint& b) const { FixedPoint result = *this; result *= b; return result; }

    FixedPoint& operator/=(const FixedPoint& b) { m_value /= b.m_value; return *this; }
    const FixedPoint operator/(const FixedPoint& b) const { FixedPoint result = *this; result /= b; return result; }

    FixedPoint& operator%=(const FixedPoint& b) { m_value %= b.m_value; return *this; }
    const FixedPoint operator%(const FixedPoint& b) const { FixedPoint result = *this; result %= b; return result; }

    bool operator == (const FixedPoint& b) { return m_value == b.m_value; }
    bool operator != (const FixedPoint& b) { return m_value != b.m_value; }
    bool operator < (const FixedPoint& b) { return m_value < b.m_value; }
    bool operator > (const FixedPoint& b) { return m_value > b.m_value; }
    bool operator <= (const FixedPoint& b) { return m_value <= b.m_value; }
    bool operator >= (const FixedPoint& b) { return m_value >= b.m_value; }


    T getInteger()
    {
        return m_value / MUL;
    }

    T getFraction()
    {
        return m_value % MUL;
    }

    float asFloat()
    {
        return (float)m_value / MUL;
    }
    
    const char* asString()
    {
        static char buffer[16] = { 0 };
        sprintf(buffer, "%u.%u", getInteger(), getFraction());
        return buffer;
    }

private:

    T m_value;
};

typedef FixedPoint<uint32_t, 100> Fixed2;
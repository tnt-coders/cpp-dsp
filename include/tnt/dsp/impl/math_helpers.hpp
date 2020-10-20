#pragma once

namespace tnt::dsp::impl
{

// Returns true if the provided value is even
constexpr bool IsEven(const size_t value);

// Returns true if the provided value is a power of 2
constexpr bool IsPowerOf2(const size_t value);

// Returns the next power of 2 greater than or equal to the provided value
constexpr size_t NextPowerOf2(const size_t value);

constexpr bool IsEven(const size_t value)
{
    return !(value % 2);
}

constexpr bool IsPowerOf2(const size_t value)
{
    return value && (!(value & (value - 1)));
}

constexpr size_t NextPowerOf2(const size_t value)
{
    size_t result = 1;
    while (result < value)
    {
        result <<= 1;
    }

    return result;
}

} /* namespace tnt::dsp::impl */
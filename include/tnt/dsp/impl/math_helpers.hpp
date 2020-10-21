#pragma once

namespace tnt::dsp::impl
{

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
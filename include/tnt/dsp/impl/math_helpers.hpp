#pragma once

namespace tnt::dsp::impl
{

constexpr bool is_even(const size_t value)
{
    return !(value % 2);
}

constexpr bool is_power_of_2(const size_t value)
{
    return value && (!(value & (value - 1)));
}

constexpr size_t next_power_of_2(const size_t value)
{
    size_t result = 1;
    while (result < value)
    {
        result <<= 1;
    }

    return result;
}

}  // namespace tnt::dsp::impl

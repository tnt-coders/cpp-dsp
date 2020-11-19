#include "approx.hpp"
#include <catch2/catch_approx.hpp>

Catch::Approx approx(const double value)
{
    return Catch::Approx{ value }.scale(1);
}
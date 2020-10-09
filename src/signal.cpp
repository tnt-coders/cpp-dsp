#include "signal.hpp"

#include <memory>
#include <vector>

namespace tnt::dsp
{

using std::make_unique;
using std::vector;

struct Signal::Impl
{
    Impl() = default;
    ~Impl() = default;

private:
    size_t m_sampleRate;
    vector<double> m_data;
};

Signal::Signal() : m_impl(make_unique<Impl>()) {}

} /* namespace tnt::dsp */
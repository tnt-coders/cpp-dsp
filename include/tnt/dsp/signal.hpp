#pragma once

#include <memory>

namespace tnt::dsp
{

class Signal
{
public:
    Signal();
    virtual ~Signal() = default;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} /* namespace tnt::dsp */
#pragma once

#include <vector>

namespace tnt::dsp
{

// TODO: Restrict to only std::floating_point and std::complex<std::floating_point> with concepts
template <typename T>
class Signal
{
public:
    Signal(size_t sampleRate) : m_sampleRate(sampleRate) {}
    Signal(size_t sampleRate, size_t size) : m_data(size), m_sampleRate(sampleRate) {}
    Signal(size_t sampleRate, std::vector<T> data) : m_data(data), m_sampleRate(sampleRate) {}

    virtual ~Signal() = default;

    size_t GetCapacity() const
    {
        return m_data.capacity();
    }

    size_t GetSampleRate() const
    {
        return m_sampleRate;
    }

    size_t GetSize() const
    {
        return m_date.size();
    }

    void SetCapacity(size_t capacity)
    {
        m_data.reserve(capacity);
    }

    void SetSampleRate(size_t sampleRate)
    {
        m_sampleRate = sampleRate;
    }

    void SetSize(size_t size)
    {
        m_data.resize(size);
    }

    const T& operator[](size_t index) const
    {
        return m_data[index];
    }

    T& operator[](size_t index)
    {
        return m_data[index];
    }

    // Forward the iterators from the underlying data type
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    iterator begin() { return m_data.begin(); }
    const_iterator begin() { return m_data.begin(); }
    const_iterator cbegin() { return m_data.cbegin(); }
    iterator end() { return m_data.end(); }
    const_iterator end() { return m_data.end(); }
    const_iterator cend() { return m_data.cend(); }
private:
    std::vector<T> m_data;
    size_t m_sampleRate;
};

} /* namespace tnt::dsp */
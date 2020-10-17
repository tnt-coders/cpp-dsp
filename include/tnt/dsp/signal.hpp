#pragma once

#include <vector>

namespace tnt::dsp
{

template <typename T>
class Signal
{
public:

    Signal(const size_t sampleRate);

    Signal(const size_t sampleRate, const size_t size);

    Signal(const Signal<T>& signal);

    virtual ~Signal();

    size_t GetCapacity() const;

    size_t GetSampleRate() const;

    size_t GetSize() const;

    void SetCapacity(size_t capacity);

    void SetSampleRate(size_t sampleRate);

    void SetSize(size_t size);

    const T& operator[](size_t index) const;

    T& operator[](size_t index);

    // Iterator functions
    using iterator = typename std::vector<T>::iterator;

    using const_iterator = typename std::vector<T>::const_iterator;

    iterator begin();

    const_iterator begin() const;

    const_iterator cbegin() const;

    iterator end();

    const_iterator end() const;

    const_iterator cend() const;

private:

    std::vector<T> m_data;

    size_t m_sampleRate;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
Signal<T>::Signal(const size_t sampleRate)
    : m_sampleRate(sampleRate)
{}

template <typename T>
Signal<T>::Signal(const size_t sampleRate, const size_t size)
    : m_data(size)
    , m_sampleRate(sampleRate)
{}

template <typename T>
Signal<T>::Signal(const Signal& signal)
    : m_data(signal.begin(), signal.end())
    , m_sampleRate(signal.GetSampleRate())
{}

template <typename T>
Signal<T>::~Signal() = default;

template <typename T>
size_t Signal<T>::GetCapacity() const
{
    return m_data.capacity();
}

template <typename T>
size_t Signal<T>::GetSampleRate() const
{
    return m_sampleRate;
}

template <typename T>
size_t Signal<T>::GetSize() const
{
    return m_data.size();
}

template <typename T>
void Signal<T>::SetCapacity(size_t capacity)
{
    m_data.reserve(capacity);
}

template <typename T>
void Signal<T>::SetSampleRate(size_t sampleRate)
{
    m_sampleRate = sampleRate;
}

template <typename T>
void Signal<T>::SetSize(size_t size)
{
    m_data.resize(size);
}

template <typename T>
const T& Signal<T>::operator[](size_t index) const
{
    return m_data[index];
}

template <typename T>
T& Signal<T>::operator[](size_t index)
{
    return m_data[index];
}

// Iterator functions
template <typename T>
typename Signal<T>::iterator Signal<T>::begin()
{
    return m_data.begin();
}

template <typename T>
typename Signal<T>::const_iterator Signal<T>::begin() const
{
    return m_data.begin();
}

template <typename T>
typename Signal<T>::const_iterator Signal<T>::cbegin() const
{
    return m_data.cbegin();
}

template <typename T>
typename Signal<T>::iterator Signal<T>::end()
{
    return m_data.end();
}

template <typename T>
typename Signal<T>::const_iterator Signal<T>::end() const
{
    return m_data.end();
}

template <typename T>
typename Signal<T>::const_iterator Signal<T>::cend() const
{
    return m_data.cend();
}

} /* namespace tnt::dsp */
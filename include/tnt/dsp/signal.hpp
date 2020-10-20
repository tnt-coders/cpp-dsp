#pragma once

#include <vector>

//TODO: Add additional container function support

namespace tnt::dsp
{

/*!
\brief Represents a DSP signal to store and process sampled data
*/
template <typename T>
class Signal
{
public:

    /*!
    \brief Constant iterator
    */
    using const_iterator = typename std::vector<T>::const_iterator;

    /*!
    \brief Iterator
    */
    using iterator = typename std::vector<T>::iterator;

    /*!
    \brief Constructor
    \param[in] sampleRate Sample rate
    */
    Signal(const size_t sampleRate)
        : m_sampleRate(sampleRate)
    {}

    /*!
    \brief Constructor
    \param[in] sampleRate Sample rate
    \param[in] size Size
    */
    Signal(const size_t sampleRate, const size_t size)
        : m_data(size)
        , m_sampleRate(sampleRate)
    {}

    /*!
    \brief Constructor
    \param[in] signal Signal
    */
    Signal(const Signal<T>& signal)
        : m_data(signal.begin(), signal.end())
        , m_sampleRate(signal.GetSampleRate())
    {}

    /*!
    \brief Destructor
    */
    virtual ~Signal() = default;

    /*!
    \brief Gets the sample rate
    \return Sample rate
    */
    size_t GetSampleRate() const
    {
        return m_sampleRate;
    }

    /*!
    \brief Sets the sample rate
    \param[in] sampleRate Sample rate
    */
    void SetSampleRate(size_t sampleRate)
    {
        m_sampleRate = sampleRate;
    }

    /*!
    \brief Gets an iterator to the beginning of the signal
    \return Iterator to the first sample
    */
    iterator begin()
    {
        return m_data.begin();
    }

    /*!
    \brief Gets a constant iterator to the beginning of the signal
    \return Constant iterator to the first sample
    */
    const_iterator begin() const
    {
        return m_data.begin();
    }

    /*!
    \brief Gets a constant iterator to the beginning of the signal
    \return Constant iterator to the first sample
    */
    const_iterator cbegin() const
    {
        return m_data.cbegin();
    }

    /*!
    \brief Gets an iterator to the end of the signal
    \return Iterator to the sample *following* the last sample
    */
    iterator end()
    {
        return m_data.end();
    }

    /*!
    \brief Gets a constant iterator to the end of the signal
    \return Constant iterator to the sample *following* the last sample
    */
    const_iterator end() const
    {
        return m_data.end();
    }

    /*!
    \brief Gets a constant iterator to the end of the signal
    \return Constant iterator to the sample *following* the last sample
    */
    const_iterator cend() const
    {
        return m_data.cend();
    }

    /*!
    \brief Gets the capacity
    \return Capacity
    */
    size_t capacity() const
    {
        return m_data.capacity();
    }

    /*!
    \brief Sets the capacity
    \param[in] capacity Desired capacity
    */
    void reserve(size_t capacity)
    {
        m_data.reserve(capacity);
    }

    /*!
    \brief Gets the size
    \return Size
    */
    size_t size() const
    {
        return m_data.size();
    }

    /*!
    \brief Sets the size
    \param[in] size Desired size
    */
    void resize(size_t size)
    {
        m_data.resize(size);
    }

    /*!
    \brief Appends a sample to the end of the signal

    If the capacity of the signal is not large enough to hold the sample, the capacity will be
    automatically expanded to hold the new sample.

    \param[in] sample Sample to append to the signal
    */
    void push_back(const T& sample)
    {
        m_data.push_back(sample);
    }

    /*!
    \brief Accesses the sample at the specified index
    \return Constant reference to the requsted sample
    */
    const T& operator[](size_t index) const
    {
        return m_data[index];
    }

    /*!
    \brief Accesses the sample at the specified index
    \return Reference to the requested sample
    */
    T& operator[](size_t index)
    {
        return m_data[index];
    }

private:

    std::vector<T> m_data;

    size_t m_sampleRate;
};

/*!
\brief Swaps the contents of one signal with another
\param[in] signal1 First signal to swap
\param[in] signal2 Second signal to swap
*/
template<typename T>
void swap(Signal<T>& signal1, Signal<T>& signal2)
{
    Signal<T> temp = std::move(signal1);
    signal1 = std::move(signal2);
    signal2 = std::move(temp);
}

} /* namespace tnt::dsp */
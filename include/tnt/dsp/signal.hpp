#pragma once

#include <cassert>
#include <complex>
#include <utility>
#include <vector>

// TODO: Add additional container function support
// TODO: Add arithmetic operators

namespace tnt::dsp
{

/*!
\brief Represents a DSP signal to store and process sampled data
*/
template <typename T>
class Signal final
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
    \brief Size type
    */
    using size_type = typename std::vector<T>::size_type;

    /*!
    \brief Value type
    */
    using value_type = typename std::vector<T>::value_type;

    /*!
    \brief Constructor
    \param[in] sampleRate Sample rate
    */
    explicit Signal(const size_t sampleRate)
        : m_sampleRate(sampleRate)
    {}

    /*!
    \brief Constructor
    \param[in] sampleRate Sample rate
    \param[in] size Size
    */
    explicit Signal(const size_t sampleRate, const size_type& size)
        : m_data(size)
        , m_sampleRate(sampleRate)
    {}

    /*!
    \brief Creates a complex signal from two real signals
    \param[in] real Signal containing the real samples
    \param[in] imaginary Signal containing the imaginary samples
    */
    template <typename U>
    Signal(const Signal<U>& real, const Signal<U>& imaginary)
        : m_data(real.size())
        , m_sampleRate(real.GetSampleRate())
    {
        assert(real.GetSampleRate() == imaginary.GetSampleRate());
        assert(real.size() == imaginary.size());

        for (size_t n = 0; n < m_data.size(); ++n)
        {
            m_data[n] = T{ real[n], imaginary[n] };
        }
    }

    /*!
    \brief Copy constructor
    \param[in] signal Signal
    */
    Signal(const Signal<T>& signal) = default;

    /*!
    \brief Move constructor
    \param[in] signal Signal
    */
    Signal(Signal<T>&& signal) = default;

    /*!
    \brief Copy assignment operator
    \param[in] signal Signal to assign from
    \return Signal equal to the input
    */
    Signal<T>& operator=(const Signal<T>& signal) = default;

    /*!
    \brief Move assignment operator
    \param[in] signal Signal to assign from
    \return Signal equal to the input
    */
    Signal<T>& operator=(Signal<T>&& signal) = default;

    /*!
    \brief Destructor
    */
    ~Signal() = default;

    /*!
    \brief Gets the duration of the signal in seconds
    \return Duration
    */
    double GetDuration() const
    {
        return this->size() / static_cast<double>(this->GetSampleRate());
    }

    /*!
    \brief Gets the sample rate
    \return Sample rate
    */
    size_t GetSampleRate() const
    {
        return m_sampleRate;
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
    size_type capacity() const
    {
        return m_data.capacity();
    }

    /*!
    \brief Sets the capacity
    \param[in] capacity Desired capacity
    */
    void reserve(const size_type& capacity)
    {
        m_data.reserve(capacity);
    }

    /*!
    \brief Gets the size
    \return Size
    */
    size_type size() const
    {
        return m_data.size();
    }

    /*!
    \brief Sets the size
    \param[in] size Desired size
    */
    void resize(const size_type& size)
    {
        m_data.resize(size);
    }

    /*!
    \brief Appends a sample to the end of the signal

    If the capacity of the signal is not large enough to hold the sample, the capacity will be
    automatically expanded to hold the new sample.

    \param[in] sample Sample to append to the signal
    */
    void push_back(const value_type& sample)
    {
        m_data.push_back(sample);
    }

    /*!
    \brief Accesses the sample at the specified index
    \return Constant reference to the requsted sample
    */
    const value_type& operator[](const size_type& index) const
    {
        assert(index < this->size());
        return m_data[index];
    }

    /*!
    \brief Accesses the sample at the specified index
    \return Reference to the requested sample
    */
    value_type& operator[](const size_type& index)
    {
        assert(index < this->size());
        return m_data[index];
    }

    // Friend declaration for swap
    template <typename U>
    friend void swap(Signal<U>& signal1, Signal<U>& signal2);

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
    using std::swap;
    swap(signal1.m_data, signal2.m_data);
    swap(signal1.m_sampleRate, signal2.m_sampleRate);
}

} /* namespace tnt::dsp */
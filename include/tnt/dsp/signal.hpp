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
class signal final
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
    \param[in] sample_rate Sample rate
    */
    explicit signal(const size_t sample_rate)
        : m_sample_rate(sample_rate)
    {}

    /*!
    \brief Constructor
    \param[in] sample_rate Sample rate
    \param[in] size Size
    */
    explicit signal(const size_t sample_rate, const size_type& size)
        : m_sample_rate(sample_rate)
        , m_data(size)
    {}

    /*!
    \brief Creates a complex signal from two real signals
    \param[in] real Signal containing the real samples
    \param[in] imaginary Signal containing the imaginary samples
    */
    template <typename U>
    signal(const signal<U>& real, const signal<U>& imaginary)
        : m_sample_rate(real.sample_rate())
        , m_data(real.size())
    {
        assert(real.sample_rate() == imaginary.sample_rate());
        assert(real.size() == imaginary.size());

        for (size_t n = 0; n < m_data.size(); ++n)
        {
            m_data[n] = T{real[n], imaginary[n]};
        }
    }

    /*!
    \brief Copy constructor
    \param[in] signal Signal
    */
    signal(const signal<T>& signal) = default;

    /*!
    \brief Move constructor
    \param[in] signal Signal
    */
    signal(signal<T>&& signal) = default;

    /*!
    \brief Copy assignment operator
    \param[in] signal Signal to assign from
    \return Signal equal to the input
    */
    signal<T>& operator=(const signal<T>& signal) = default;

    /*!
    \brief Move assignment operator
    \param[in] signal Signal to assign from
    \return Signal equal to the input
    */
    signal<T>& operator=(signal<T>&& signal) = default;

    /*!
    \brief Destructor
    */
    ~signal() = default;

    /*!
    \brief Gets the duration of the signal in seconds
    \return Duration
    */
    double duration() const
    {
        return this->size() / static_cast<double>(this->sample_rate());
    }

    /*!
    \brief Gets the sample rate
    \return Sample rate
    */
    size_t sample_rate() const
    {
        return m_sample_rate;
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

    // Friend declaration for swap
    template <typename U>
    friend void swap(signal<U>& signal1, signal<U>& signal2);

private:
    size_t         m_sample_rate;
    std::vector<T> m_data;
};

/*!
\brief Swaps the contents of one signal with another
\param[in] signal1 First signal to swap
\param[in] signal2 Second signal to swap
*/
template <typename T>
void swap(signal<T>& signal1, signal<T>& signal2)
{
    using std::swap;
    swap(signal1.m_sample_rate, signal2.m_sample_rate);
    swap(signal1.m_data, signal2.m_data);
}

/*
\brief Generates a complex signal from two real signals
\param[in] real Signal containing the real samples
\param[in] imaginary Signal containing the imaginary samples
\return Complex signal created from the real and imaginary parts
*/
template <typename T>
signal<std::complex<T>> complex_signal(signal<T> real, signal<T> imaginary)
{
    return dsp::signal<std::complex<T>>(real, imaginary);
}

}  // namespace tnt::dsp

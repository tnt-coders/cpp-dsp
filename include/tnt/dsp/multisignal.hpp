#pragma once

#include "signal.hpp"

#include <cassert>
#include <initializer_list>
#include <vector>

namespace tnt::dsp
{

/*!
\brief Represents a multi-channel DSP signal to store and process sampled data
*/
template <typename T>
class Multisignal final
{
public:
    /*!
    \brief Constant iterator
    */
    using const_iterator = typename std::vector<std::vector<T>>::const_iterator;

    /*!
    \brief Iterator
    */
    using iterator = typename std::vector<std::vector<T>>::iterator;

    /*!
    \brief Size type
    */
    using size_type = typename std::vector<std::vector<T>>::size_type;

    /*!
    \brief Value type
    */
    using value_type = typename std::vector<std::vector<T>>::value_type;

    /*!
    \brief Constructor
    \param[in] signals One or more single channel signals
    */
    Multisignal(const std::initializer_list<Signal<T>> signals)
        : m_sample_rate()
        , m_data()
    {
        for (const auto& signal : signals)
        {
            this->add_channel(signal);
        }
    }

    /*
    \brief Constructor
    \param[in] sample_rate Sample rate
    */
    explicit Multisignal(const size_t sample_rate)
        : m_sample_rate(sample_rate)
        , m_data()
    {}

    /*
    \brief Constructor
    \param[in] sample_rate Sample rate
    \param[in] size Size
    */
    explicit Multisignal(const size_t sample_rate, const size_type& size)
        : m_sample_rate(sample_rate)
        , m_data(size)
    {}

    /*!
    \brief Constructor
    \param[in] sample_rate Sample rate
    \param[in] size Size
    \param[in] channels Number of channels
    */
    explicit Multisignal(const size_t                              sample_rate,
                         const size_type&                          size,
                         const typename std::vector<T>::size_type& channels)
        : m_sample_rate(sample_rate)
        , m_data(size, std::vector<T>(channels))
    {}

    /*!
    \brief Copy constructor
    \param[in] signal Multi-channel signal
    */
    Multisignal(const Multisignal& signal) = default;

    /*!
    \brief Move constructor
    \param[in] signal Multi-channel signal
    */
    Multisignal(Multisignal<T>&& signal) = default;

    /*!
    \brief Copy assignment operator
    \param[in] signal Multi-channel signal to assign from
    \return Multi-channel signal equal to the input
    */
    Multisignal<T>& operator=(const Multisignal<T>& signal) = default;

    /*!
    \brief Move assignment operator
    \param[in] signal Multi-channel signal to assign from
    \return Multi-channel signal equal to the input
    */
    Multisignal<T>& operator=(Multisignal<T>&& signal) = default;

    /*!
    \brief Destructor
    */
    ~Multisignal() = default;

    /*!
    \brief Gets the specified channel
    \param[in] channel Desired channel
    \return Signal contained in the specified channel
    */
    Signal<T> channel(const typename std::vector<T>::size_type& channel) const
    {
        assert(channel < this->channels());

        Signal<T> signal(this->sample_rate(), this->size());
        for (size_type n = 0; n < this->size(); ++n)
        {
            signal[n] = m_data[n][channel];
        }

        return signal;
    }

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
    \brief Accesses the samples at the specified index
    \return Constant reference to the requested samples
    */
    const value_type& operator[](const size_type& index) const
    {
        assert(index < this->size());
        return m_data[index];
    }

    /*!
    \brief Accesses samples at the specified index
    \return Reference to the requested samples
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
    \brief Adds a channel
    \param[in] signal Signal to put in the channel
    */
    void add_channel(const Signal<T>& signal)
    {
        // Validate sample rate
        if (this->sample_rate())
        {
            assert(signal.sample_rate() == this->sample_rate());
        }
        else
        {
            m_sample_rate = signal.sample_rate();
        }

        // Validate size
        if (this->size())
        {
            assert(signal.size() == this->size());
        }
        else
        {
            m_data.resize(signal.size());
        }

        for (size_type n = 0; n < this->size(); ++n)
        {
            m_data[n].push_back(signal[n]);
        }
    }

    /*!
    \brief Gets the number of samples per channel
    \return Number of samples per channel
    */
    size_type size() const
    {
        return m_data.size();
    }

    /*!
    \brief Gets the number of channels in the signal
    \return Number of channels in the signal
    */
    size_type channels() const
    {
        return m_data.size() == 0 ? 0 : m_data[0].size();
    }

    // Friend declaration for swap
    template <typename U>
    friend void swap(Multisignal<U>& signal1, Multisignal<U>& signal2);

private:
    size_t                      m_sample_rate;
    std::vector<std::vector<T>> m_data;
};

/*!
\brief Swaps the contents of one signal with another
\param[in] signal1 First signal to swap
\param[in] signal2 Second signal to swap
*/
template <typename T>
void swap(Multisignal<T>& signal1, Multisignal<T>& signal2)
{
    using std::swap;
    swap(signal1.m_sample_rate, signal2.m_sample_rate);
    swap(signal1.m_data, signal2.m_data);
}

} /* namespace tnt::dsp */

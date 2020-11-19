#pragma once

#include "signal.hpp"
#include <cassert>
#include <initializer_list>
#include <vector>

namespace tnt::dsp
{

/*!
\brief Represents a Multi-channel DSP signal to store and process sampled data
*/
template <typename T>
class multisignal final
{
    class signal_proxy;
public:

    /*!
    \brief Size type
    */
    using size_type = typename std::vector<signal<T>>::size_type;

    /*!
    \brief Value type
    */
    using value_type = typename std::vector<signal<T>>::value_type;

    /*!
    \brief Constructor
    \param[in] signals One or more single channel signals
    */
    explicit multisignal(const std::initializer_list<signal<T>> signals)
        : m_sample_rate(0)
        , m_size(0)
        , m_channels(signals)
    {
        for (const auto& signal : signals)
        {
            // Validate sample rate
            if (m_sample_rate)
            {
                assert(signal.sample_rate() == m_sample_rate);
            }
            else
            {
                m_sample_rate = signal.sample_rate();
            }

            // Validate size
            if (m_size)
            {
                assert(signal.size() == m_size);
            }
            else
            {
                m_size = signal.size();
            }
        }
    }

    /*
    \brief Constructor
    \param[in] sample_rate Sample rate
    */
    explicit multisignal(const size_t sample_rate)
        : m_sample_rate(sample_rate)
        , m_size(0)
        , m_channels()
    {}

    /*
    \brief Constructor
    \param[in] sample_rate Sample rate
    \param[in] size Size
    */
    explicit multisignal(const size_t sample_rate, const typename signal<T>::size_type& size)
        : m_sample_rate(sample_rate)
        , m_size(size)
        , m_channels()
    {}

    /*!
    \brief Constructor
    \param[in] sample_rate Sample rate
    \param[in] size Size
    \param[in] channels Number of channels
    */
    explicit multisignal(const size_t sample_rate, const typename signal<T>::size_type& size, const size_type& channels)
        : m_sample_rate(sample_rate)
        , m_size(size)
        , m_channels(channels, signal<T>{ sample_rate, size })
    {}

    /*!
    \brief Copy constructor
    \param[in] signal Multi-channel signal
    */
    multisignal(const multisignal& signal) = default;

    /*!
    \brief Move constructor
    \param[in] signal Multi-channel signal
    */
    multisignal(multisignal<T>&& signal) = default;

    /*!
    \brief Copy assignment operator
    \param[in] signal Multi-channel signal to assign from
    \return Multi-channel signal equal to the input
    */
    multisignal<T>& operator=(const multisignal<T>& signal) = default;

    /*!
    \brief Move assignment operator
    \param[in] signal Multi-channel signal to assign from
    \return Multi-channel signal equal to the input
    */
    multisignal<T>& operator=(multisignal<T>&& signal) = default;

    /*!
    \brief Destructor
    */
    ~multisignal() = default;

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
    \brief Accesses the channel at the specified index
    \return Constant reference to the requsted channel
    */
    const signal<T>& operator[](const size_type& channel) const
    {
        assert(channel < this->channels());
        return m_channels[channel];
    }

    /*!
    \brief Accesses the channel at the specified index
    \return Proxy reference to the requested channel (The proxy prevents resizing)
    */
    signal_proxy operator[](const size_type& channel)
    {
        assert(channel < this->channels());
        return m_channels[channel];
    }

    /*!
    \brief Adds a channel
    \param[in] signal Signal to put in the channel
    */
    void add_channel(const signal<T>& signal)
    {
        // Validate sample rate
        if (m_sample_rate)
        {
            assert(signal.sample_rate() == m_sample_rate);
        }
        else
        {
            m_sample_rate = signal.sample_rate();
        }

        // Validate size
        if (m_size)
        {
            assert(signal.size() == m_size);
        }
        else
        {
            m_size = signal.size();
        }

        m_channels.push_back(signal);
    }

    /*!
    \brief Gets the number of channels in the signal
    \return Number of channels in the signal
    */
    size_type channels() const
    {
        return m_channels.size();
    }

    /*!
    \brief Gets the number of samples per channel
    \return Number of samples per channel
    */
    size_type size() const
    {
        return m_size;
    }

    // Friend declaration for swap
    template <typename U>
    friend void swap(multisignal<U>& signal1, multisignal<U>& signal2);

private:

    // Proxy class that provides a limited interface to the signal class
    // We don't want to allow users to modify the size of a signal contained within a multi-signal.
    class signal_proxy final
    {
    public:
        using const_iterator = typename signal<T>::const_iterator;
        using iterator = typename signal<T>::iterator;
        using size_type = typename signal<T>::size_type;
        using value_type = typename signal<T>::value_type;
        signal_proxy(signal<T>& signal) : m_signal(signal) {}
        operator signal<T>() { return m_signal; }
        double duration() const { return m_signal.duration(); }
        size_t sample_rate() const { return m_signal.sample_rate(); }
        iterator begin() { return m_signal.begin(); }
        const_iterator begin() const { return m_signal.begin(); }
        const_iterator cbegin() const { return m_signal.cbegin(); }
        iterator end() { return m_signal.end(); }
        const_iterator end() const { return m_signal.end(); }
        const_iterator cend() const { return m_signal.cend(); }
        size_type size() const { return m_signal.size(); }
        const value_type& operator[](const size_type index) const { return m_signal[index]; }
        typename value_type& operator[](const size_type index) { return m_signal[index]; }
    private:
        signal<T>& m_signal;
    };

    size_t m_sample_rate;
    size_type m_size;
    std::vector<signal<T>> m_channels;
};

/*!
\brief Swaps the contents of one signal with another
\param[in] signal1 First signal to swap
\param[in] signal2 Second signal to swap
*/
template<typename T>
void swap(multisignal<T>& signal1, multisignal<T>& signal2)
{
    using std::swap;
    swap(signal1.m_channels, signal2.m_channels);
}


} /* namespace tnt::dsp */
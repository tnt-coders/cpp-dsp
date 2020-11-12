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
class MultiSignal final
{
    class SignalProxy;
public:

    /*!
    \brief Constant iterator
    */
    using const_iterator = typename std::vector<Signal<T>>::const_iterator;

    /*!
    \brief Iterator
    */
    using iterator = typename std::vector<Signal<T>>::iterator;

    /*!
    \brief Size type
    */
    using size_type = typename std::vector<Signal<T>>::size_type;

    /*!
    \brief Value type
    */
    using value_type = typename std::vector<Signal<T>>::value_type;

    /*!
    \brief Constructor
    \param[in] signals One or more single channel signals
    */
    explicit MultiSignal(const std::initializer_list<Signal<T>> signals)
        : m_sampleRate(0)
        , m_size(0)
        , m_channels(signals)
    {
        for (const auto& signal : signals)
        {
            // Validate sample rate
            if (m_sampleRate)
            {
                assert(signal.GetSampleRate() == m_sampleRate);
            }
            else
            {
                m_sampleRate = signal.GetSampleRate();
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
    \param[in] sampleRate Sample rate
    */
    explicit MultiSignal(const size_t sampleRate)
        : m_sampleRate(sampleRate)
        , m_size(0)
        , m_channels()
    {}

    /*
    \brief Constructor
    \param[in] sampleRate Sample rate
    \param[in] size Size
    */
    explicit MultiSignal(const size_t sampleRate, const typename Signal<T>::size_type& size)
        : m_sampleRate(sampleRate)
        , m_size(size)
        , m_channels()
    {}

    /*!
    \brief Constructor
    \param[in] sampleRate Sample rate
    \param[in] size Size
    \param[in] channels Number of channels
    */
    explicit MultiSignal(const size_t sampleRate, const typename Signal<T>::size_type& size, const size_type& channels)
        : m_sampleRate(sampleRate)
        , m_size(size)
        , m_channels(channels, Signal<T>{ sampleRate, size })
    {}

    /*!
    \brief Copy constructor
    \param[in] signal Multi-channel signal
    */
    MultiSignal(const MultiSignal& signal) = default;

    /*!
    \brief Move constructor
    \param[in] signal Multi-channel signal
    */
    MultiSignal(MultiSignal<T>&& signal) = default;

    /*!
    \brief Copy assignment operator
    \param[in] signal Multi-channel signal to assign from
    \return Multi-channel signal equal to the input
    */
    MultiSignal<T>& operator=(const MultiSignal<T>& signal) = default;

    /*!
    \brief Move assignment operator
    \param[in] signal Multi-channel signal to assign from
    \return Multi-channel signal equal to the input
    */
    MultiSignal<T>& operator=(MultiSignal<T>&& signal) = default;

    /*!
    \brief Destructor
    */
    ~MultiSignal() = default;

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
    \brief Adds a channel
    \param[in] signal Signal to put in the channel
    */
    void AddChannel(const Signal<T>& signal)
    {
        // Validate sample rate
        if (m_sampleRate)
        {
            assert(signal.GetSampleRate() == m_sampleRate);
        }
        else
        {
            m_sampleRate = signal.GetSampleRate();
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
    \brief Gets the size
    \return Size
    */
    size_type size() const
    {
        return m_size;
    }

    /*!
    \brief Accesses the channel at the specified index
    \return Constant reference to the requsted channel
    */
    const Signal<T>& operator[](const size_type& channel) const
    {
        assert(channel < this->channels());
        return m_channels[channel];
    }

    /*!
    \brief Accesses the channel at the specified index
    \return Proxy reference to the requested channel (The proxy prevents resizing)
    */
    SignalProxy operator[](const size_type& channel)
    {
        assert(channel < this->channels());
        return m_channels[channel];
    }

    // Friend declaration for swap
    template <typename U>
    friend void swap(MultiSignal<U>& signal1, MultiSignal<U>& signal2);

private:

    // Proxy class that provides a limited interface to the Signal class
    // We don't want to allow users to modify the size of a signal contained within a multi-signal.
    class SignalProxy final
    {
    public:
        using const_iterator = typename Signal<T>::const_iterator;
        using iterator = typename Signal<T>::iterator;
        using size_type = typename Signal<T>::size_type;
        using value_type = typename Signal<T>::value_type;
        SignalProxy(Signal<T>& signal) : m_signal(signal) {}
        operator Signal<T>() { return m_signal; }
        double GetDuration() const { return m_signal.GetDuration(); }
        size_t GetSampleRate() const { return m_signal.GetSampleRate(); }
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
        Signal<T>& m_signal;
    };

    size_t m_sampleRate;
    size_type m_size;
    std::vector<Signal<T>> m_channels;
};

/*!
\brief Swaps the contents of one signal with another
\param[in] signal1 First signal to swap
\param[in] signal2 Second signal to swap
*/
template<typename T>
void swap(MultiSignal<T>& signal1, MultiSignal<T>& signal2)
{
    using std::swap;
    swap(signal1.m_channels, signal2.m_channels);
}


} /* namespace tnt::dsp */
#include "DiscretePeriodicWavelet.h"

#include <wavelet2d/wavelet2d.h>

void DiscretePeriodicWavelet::setSignal(const SignalSource &data)
{
    m_inputSignal = data.toStdVector();
}

AudioSignal::SignalSource DiscretePeriodicWavelet::inputSignal() const
{
    return SignalSource::fromStdVector(m_inputSignal);
}

void DiscretePeriodicWavelet::makeTransform()
{
    m_flag.clear();
    m_length.clear();
    m_transformedSignal.clear();
    m_outputSignal.clear();

    if (m_inputSignal.empty()) {
        return;
    }

    dwt(m_inputSignal, m_level, toStdString(m_waveletFunction), m_transformedSignal, m_flag, m_length);
}

AudioSignal::SignalSource DiscretePeriodicWavelet::transformedSignal() const
{
    return SignalSource::fromStdVector(m_transformedSignal);
}

AudioSignal::SignalsSourceVector DiscretePeriodicWavelet::transformedSignalVector() const
{
    if (m_transformedSignal.empty())
        return SignalsSourceVector();

    auto begin = 0;
    auto end = 0;
    SignalsSourceVector decomposition;
    for (size_t i = 0; i < m_length.size() - 1; ++i) {
        auto currentSize = m_length[i];
        SignalSource currentSignal;
        end = begin + currentSize;
        for (int64_t j = begin; j < end; j++) {
            currentSignal.push_back(m_transformedSignal[j]);
        }
        decomposition.push_back(currentSignal);
        begin = end;
    }

    return decomposition;
}

void DiscretePeriodicWavelet::setTransformedSignalVector(const SignalsSourceVector &data)
{
    SignalSource thresholded;
    for (auto i : data) {
        thresholded.append(i);
    }
    m_thresholdedSignal = thresholded.toStdVector();
}

AudioSignal::SignalSource DiscretePeriodicWavelet::thresholdedSignal() const
{
    return SignalSource::fromStdVector(m_thresholdedSignal);
}

void DiscretePeriodicWavelet::makeInverseTransform()
{
    if (m_thresholdedSignal.empty()) {
        if (m_transformedSignal.empty()) {
            return;
        }
        idwt(m_transformedSignal, m_flag, toStdString(m_waveletFunction), m_outputSignal, m_length);
        return;
    }

    idwt(m_thresholdedSignal, m_flag, toStdString(m_waveletFunction), m_outputSignal, m_length);
}

AudioSignal::SignalSource DiscretePeriodicWavelet::outputSignal() const
{
    return SignalSource::fromStdVector(m_outputSignal);
}

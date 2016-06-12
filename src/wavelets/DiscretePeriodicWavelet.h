#pragma once

#include "Wavelet.h"
#include <src/AudioSignal.h>

class DiscretePeriodicWavelet
{
public:
    using SignalSource = AudioSignal::SignalSource;
    using SignalsSourceVector = AudioSignal::SignalsSourceVector;

    DiscretePeriodicWavelet() {}
    ~DiscretePeriodicWavelet() {}

    void setSignal(const SignalSource &data);
    SignalSource inputSignal() const;

    void setWaveletFunction(Wavelet::WaveletFunction function) { m_waveletFunction = function; }
    Wavelet::WaveletFunction waveletFunction() const { return m_waveletFunction; }
    void setLevel(int level) { m_level = level; }
    int level() const { return m_level; }

    void makeTransform();

    SignalSource transformedSignal() const;
    SignalsSourceVector transformedSignalVector() const;
    void setTransformedSignalVector(const SignalsSourceVector &data);
    SignalSource thresholdedSignal() const;

    void makeInverseTransform();

    SignalSource outputSignal() const;

private:
    std::vector<double> m_inputSignal;

    Wavelet::WaveletFunction m_waveletFunction = Wavelet::WaveletFunction::Haar;
    int m_level = 1;

    std::vector<double> m_flag;
    std::vector<int> m_length;
    std::vector<double> m_transformedSignal;
    std::vector<double> m_thresholdedSignal;

    std::vector<double> m_outputSignal;
};

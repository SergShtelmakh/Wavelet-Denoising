#pragma once

#include <QObject>

#include <src/AudioSignal.h>

#include <QScopedPointer>

class DiscretePeriodicWavelet;
class ThresholdsManager;

class DenoisingManager : public QObject
{
public:
    using SignalSource = AudioSignal::SignalSource;
    using SignalsSourceVector = AudioSignal::SignalsSourceVector;

    DenoisingManager();

    void setSignal(const AudioSignal& signal);
    void prepareToDenoising(const QString& waveletName, int level);
    void denoising(const QString& thresholdType, const QVector<double>& thresholds);

    SignalSource inputSignal() const { return m_inputSignal; }
    SignalSource transformedSignal() const { return m_transformedSignal; }
    SignalSource outputSignal() const { return m_outputSignal; }

    SignalsSourceVector transformedDecomposition() const;
    SignalSource thresholdedSignal() const;

private:
    SignalSource m_inputSignal;
    SignalSource m_transformedSignal;
    SignalsSourceVector m_transformedSignalVector;
    SignalSource m_outputSignal;

    QScopedPointer<DiscretePeriodicWavelet> m_wavelet;
    QScopedPointer<ThresholdsManager> m_thresholdsManager;
};

#pragma once

#include <QObject>

#include <src/AudioSignal.h>
#include <src/ThresholdsManager.h>
#include <src/wavelets/DiscretePeriodicWavelet.h>
#include <QScopedPointer>

class DenoisingManager : public QObject
{
public:
    using SignalSource = AudioSignal::SignalSource;
    using SignalsSourceVector = AudioSignal::SignalsSourceVector;

    DenoisingManager();

    void setSignal(const AudioSignal& signal);
    void prepareToDenoising(const QString& waveletName, int level);
    void prepareToDenoising(Wavelet::WaveletFunction name, int level);
    void denoising(const QString& thresholdType, const QVector<double>& thresholds);
    void automaticDenoising(ThresholdsManager::ThresholdType type);

    SignalSource noisedSignal() const { return m_noisedSignal; }
    SignalSource transformedSignal() const { return m_transformedSignal; }
    SignalSource denoisedSignal() const { return m_denoisedSignal; }

    SignalsSourceVector transformedDecomposition() const;
    SignalSource thresholdedSignal() const;

private:
    SignalSource m_noisedSignal;
    SignalSource m_transformedSignal;
    SignalsSourceVector m_transformedSignalVector;
    SignalSource m_denoisedSignal;

    QScopedPointer<DiscretePeriodicWavelet> m_wavelet;
    QScopedPointer<ThresholdsManager> m_thresholdsManager;
};

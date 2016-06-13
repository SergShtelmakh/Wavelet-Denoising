#include "DenoisingManager.h"

DenoisingManager::DenoisingManager()
    : m_wavelet(new DiscretePeriodicWavelet)
    , m_thresholdsManager(new ThresholdsManager)
{
}

void DenoisingManager::setSignal(const AudioSignal &signal)
{
    m_noisedSignal = signal.source();
}

void DenoisingManager::setSignalSource(const DenoisingManager::SignalSource &signalSource)
{
    m_noisedSignal = signalSource;
}

void DenoisingManager::prepareToDenoising(const QString &waveletName, int level)
{
    prepareToDenoising(Wavelet::fromString(waveletName), level);
}

void DenoisingManager::prepareToDenoising(Wavelet::WaveletFunction name, int level)
{
    m_wavelet->setWaveletFunction(name);
    m_wavelet->setLevel(level);
    m_wavelet->setSignal(m_noisedSignal);

    m_wavelet->makeTransform();
    m_transformedSignal = m_wavelet->transformedSignal();
    m_transformedSignalVector = m_wavelet->transformedSignalVector();
}

void DenoisingManager::denoising(const QString &thresholdType, const QVector<double> &thresholds)
{
    m_thresholdsManager->setSignalsVector(m_transformedSignalVector);
    m_thresholdsManager->setThresholdType(ThresholdsManager::fromString(thresholdType));
    m_thresholdsManager->makeThreshold(thresholds);

    m_wavelet->setTransformedSignalVector(m_thresholdsManager->thresholdedSignalsVector());
    m_wavelet->makeInverseTransform();
    m_denoisedSignal = m_wavelet->outputSignal();
}

void DenoisingManager::automaticDenoising(ThresholdsManager::ThresholdType type)
{
    m_thresholdsManager->setSignalsVector(m_transformedSignalVector);
    m_thresholdsManager->automaticThreshold(type);

    m_wavelet->setTransformedSignalVector(m_thresholdsManager->thresholdedSignalsVector());

    m_wavelet->makeInverseTransform();
    m_denoisedSignal = m_wavelet->outputSignal();
}

AudioSignal::SignalsSourceVector DenoisingManager::transformedDecomposition() const
{
    return m_wavelet->transformedSignalVector();
}

AudioSignal::SignalSource DenoisingManager::thresholdedSignal() const
{
    return m_wavelet->thresholdedSignal();
}


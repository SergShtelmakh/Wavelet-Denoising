#include "SignalAnalyzer.h"

#include <src/AudioUtil.h>
#include <src/ThresholdsManager.h>
#include <src/DenoisingManager.h>

// TODO move to ui
namespace {
    static const QVector<Wavelet::WaveletFunction> testWavelets = {
        Wavelet::Biorthogonal1_1
    };

    static const QVector<ThresholdsManager::ThresholdType> testThresholds = {
        ThresholdsManager::Hard,
        ThresholdsManager::Soft,
        ThresholdsManager::CustomHard,
        ThresholdsManager::CustomSoft
    };
}


SignalAnalyzer::SignalAnalyzer(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<QVector<double>>();
}

void SignalAnalyzer::setSignal(const SignalAnalyzer::SignalSource &signal)
{
    m_inputSignal = signal;
}

void SignalAnalyzer::setNoiseLevels(int min, int max, int step)
{
    m_noiseMin = min;
    m_noiseMax = max;
    m_noiseStep = step;
}

void SignalAnalyzer::start()
{
    if (m_noiseMax <= m_noiseMin) {
        qWarning("SignalAnalyzer::start m_noiseMax <= m_noiseMin");
        return;
    }

    if (m_noiseStep > m_noiseMax - m_noiseMin) {
        qWarning("SignalAnalyzer::start m_noiseStep > m_noiseMax - m_noiseMin");
        return;
    }

    emit started();

    for (auto wavelet : testWavelets) {
        if (m_isStopped) {
            return;
        }

        analyzeWavelet(wavelet);
    }
}

void SignalAnalyzer::stop()
{
    m_isStopped = true;

    emit finished();
}

void SignalAnalyzer::analyzeWavelet(Wavelet::WaveletFunction waveletType)
{
    emit log(QString("Start analyze %1").arg(Wavelet::toString(waveletType)));

    SnrPlots plotData;

    for (int noiseLevel = m_noiseMin; noiseLevel <= m_noiseMax; noiseLevel += m_noiseStep) {
        m_processedSignal = AudioUtil::makeWhiteNoise(m_inputSignal, noiseLevel);
        for (auto thresholdsType: testThresholds) {
            if (m_isStopped) {
                return;
            }

            DenoisingManager manager;
            manager.setSignalSource(m_processedSignal);
            manager.prepareToDenoising(waveletType, 5);
            manager.automaticDenoising(thresholdsType);
            auto snr = AudioUtil::SNR(manager.denoisedSignal(), m_inputSignal);
            plotData[thresholdsType].append(snr);
            emit log(QString("Processed level=%1    thresholdType=%2    snr=%3").arg(noiseLevel).arg(ThresholdsManager::toString(thresholdsType)).arg(snr));
        }
    }

    for (auto key : plotData.keys()) {
        emit printResut(Wavelet::toString(waveletType), ThresholdsManager::toString(key), plotData.value(key));
    }
    emit printCurrentResult();
}

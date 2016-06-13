#pragma once

#include <QObject>

#include <src/AudioSignal.h>
#include <src/wavelets/Wavelet.h>
#include <src/ThresholdsManager.h>
#include <QVector>

class SignalAnalyzer : public QObject
{
    Q_OBJECT
public:
    using SignalSource = AudioSignal::SignalSource;
    using SnrPlots = QHash<ThresholdsManager::ThresholdType, SignalSource>;

    explicit SignalAnalyzer(QObject *parent = 0);

    void setSignal(const SignalSource &signal);
    void setNoiseLevels(int min, int max, int step);

signals:
    void log(const QString& str);
    // TODO refactoring
    void printResut(const QString& waveletName, const QString& thresholdName, const QVector<double>& data);
    void printCurrentResult();
    void started();
    void finished();

public slots:
    void start();
    void stop();

private:
    void analyzeWavelet(Wavelet::WaveletFunction type);

    int m_noiseMin = 0;
    int m_noiseMax = 0;
    int m_noiseStep = 0;

    SignalSource m_inputSignal;
    SignalSource m_processedSignal;

    bool m_isStopped = false;
};

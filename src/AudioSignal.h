#pragma once

#include <QString>
#include <QVector>

#include <src/Constants.h>

#include <libs/aquila/global.h>
#include <libs/aquila/source/WaveFile.h>

class AudioSignal
{

public:
    using SignalSource = QVector<double>;

    AudioSignal(){}

    AudioSignal(const QString& fileName);
    AudioSignal(const SignalSource& signalSource, double sampleFrequency = Constants::audioDefaultSampleFrequency);
    void operator=(const AudioSignal& other);

    void load(const QString& fileName);
    void save(const QString& fileName);

    SignalSource source() const;
    int audioLength() const;
    double sampleFrequency() const;

    void makeWhiteNoise(double maxAmplitude = Constants::noiseDefaultMaxAmplitude, double density = Constants::noiseDefaultDensity);

private:
    void normalizeSignalSource();

    SignalSource m_signalSource;
    Aquila::WaveFile *m_file = nullptr;
    double m_sampleFrequency = Constants::audioDefaultSampleFrequency;
};

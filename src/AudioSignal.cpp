#include "AudioSignal.h"

#include <src/AudioUtil.h>

namespace {

const int cMsInSec = 1000;

AudioSignal::SignalSource toSignalSource(Aquila::WaveFile *wave)
{
    AudioSignal::SignalSource resultSignal;
    for (auto sample : *wave) {
        resultSignal.push_back(sample);
    }
    return resultSignal;
}

}

AudioSignal::AudioSignal(const QString &fileName)
{
    load(fileName);
}

AudioSignal::AudioSignal(const SignalSource &signalSource, double sampleFrequency)
{
    m_signalSource = signalSource;
    m_sampleFrequency = sampleFrequency;
}

void AudioSignal::operator=(const AudioSignal &other)
{
    m_file = &(*other.m_file);
    m_sampleFrequency = other.m_sampleFrequency;
    m_signalSource = other.m_signalSource;
}

void AudioSignal::load(const QString &str)
{
    if (m_file) {
        delete m_file;
    }
    m_file = new Aquila::WaveFile(QString(str).toStdString(), Aquila::StereoChannel::LEFT);

    m_signalSource = toSignalSource(m_file);
    m_sampleFrequency = m_file->getSampleFrequency();
}

void AudioSignal::save(const QString &str)
{
    normalizeSignalSource();
    Aquila::WaveFile::save(Aquila::SignalSource(m_signalSource.toStdVector(), m_sampleFrequency), QString(str).toStdString());
}

AudioSignal::SignalSource AudioSignal::source() const
{
    return m_signalSource;
}

int AudioSignal::audioLength() const
{
    return m_file ? m_file->getAudioLength() : m_signalSource.size() / (m_sampleFrequency / cMsInSec);
}

double AudioSignal::sampleFrequency() const
{
    return m_sampleFrequency;
}

void AudioSignal::makeWhiteNoise(double maxAmplitude, double density)
{
    m_signalSource = AudioUtil::makeWhiteNoise(m_signalSource, maxAmplitude, density);
}

void AudioSignal::normalizeSignalSource()
{
    for (auto i = 1; i < m_signalSource.size() - 1; i++) {
        if (m_signalSource[i] >= INT16_MAX) {
            m_signalSource[i] = m_signalSource[i - 1];
        }
        if (m_signalSource[i] <= INT16_MIN) {
            m_signalSource[i] = m_signalSource[i - 1];
        }
    }
}

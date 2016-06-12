#include "AudioUtil.h"

#include <QTime>

namespace AudioUtil {

int rand(int min, int max) {
    Q_ASSERT(max > min);
    return qrand() % ((max + 1) - min) + min;
}

bool genenerateRandomEvent(double probability)
{
    const int64_t maxRand = 1000;
    return rand(0, maxRand) < probability * maxRand;
}

SignalSource makeWhiteNoise(const SignalSource &signal, double maxAmplitude, double density) {
    SignalSource result;
    for (auto sample : signal) {
        if (genenerateRandomEvent(density)) {
            sample = sample + rand(-maxAmplitude, maxAmplitude);
        }
        result << sample;
    }
    return result;
}

SignalSource amplitudeAverage(const SignalSource &signal, int step, bool absolute)
{
    SignalSource result;
    int halfStep = step/2;
    for (auto i = 0; i < signal.size(); i++) {
        double sum = 0.0;
        for (auto j = i - halfStep; j < i - halfStep + step; j++) {
            if (j >= 0 && j < signal.size()) {
                sum += absolute ? qAbs(signal[j]) : signal[j];
            }
        }
        result << sum/step;
    }
    return result;
}

QString generateAudioFileName(const QString &str)
{
    return QString("%1audio%2.wav").arg(str).arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
}

}

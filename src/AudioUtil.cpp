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

double maxAmplitude(const SignalSource &signal)
{
    double max = 0.0;
    for (auto val : signal) {
        max = qMax(max, qAbs(val));
    }

    return max;
}

SignalSource signalDensity(const SignalSource &signal, bool positivePart, int max)
{
    double size_d = 0.0;
    for (auto val : signal) {
        size_d = positivePart ? qMax(size_d, val) : qMin(size_d, val);
    }

    auto size = static_cast<int>(positivePart ? size_d : -size_d);
    size = qMin(size, static_cast<int>(max));
    SignalSource result(size);
    for (auto val : signal) {
        int val_t = trunc(val);
        int index = -1;
        if (val_t > 0 && positivePart) {
            index = val_t - 1;
        } else if (val_t < 0 && !positivePart) {
            index = size + val_t - 1;
        }

        if (index >= 0 && index < result.size()) {
            result[index] = result[index] + 1;
        }
    }

    return result;
}

QString generateAudioFileName(const QString &str)
{
    return QString("%1audio%2.wav").arg(str).arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
}

}

#include "AudioUtil.h"

#include <QTime>

namespace AudioUtil {

int rand(int min, int max) {
    Q_ASSERT(max > min);
    return qrand() % ((max + 1) - min) + min;
}

double d_f(double x_1, double x_2, double x_3, double h) {
    Q_UNUSED(x_2);
    return (x_3 - x_1)/(2*h);
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

SignalSource signalDifference(const SignalSource &first, const SignalSource &second)
{
    auto size = qMin(first.size(), second.size());
    SignalSource result;
    for (int64_t i = 0 ; i < size; i++) {
        result.push_back(first[i] - second[i]);
    }
    return result;
}

SignalSource derivative(const SignalSource &source, int h, bool smooth)
{
    SignalSource result;
    for (int i = 0; i < source.size(); i ++) {
        auto d = 0.0;
        if (i - h >= 0 && i + h < source.size()) {
            d = d_f(source[i - h], source[i], source[i + h], h);
        }
        result << d;
    }

    if (smooth) {
        return amplitudeAverage(result, h*2, false);
    }
    return result;
}

double findMax(const SignalSource &signal)
{
    double max = 0.0;
    for (auto val : signal) {
        max = qMax(max, qAbs(val));
    }

    return max;
}

double findMin(const SignalSource &signal)
{
    double min = 0.0;
    for (auto val : signal) {
        min = qMin(min, val);
    }

    return min;
}

int indexOfMin(const SignalSource &signal)
{
    if (signal.empty()) {
        qDebug("Wrong signal");
        return -1;
    }
    double min = signal[0];
    int minIndex = 0;
    for (int i = 0; i < signal.size(); i++) {
        if (signal[i] < min) {
            min = signal[i];
            minIndex = i;
        }
    }
    return minIndex;
}

int findIndexOfMoreThan(const SignalSource &signal, double val, int startIndex)
{
    for (int i = startIndex; i < signal.size(); i++) {
        if (signal[i] > val) {
            return i;
        }
    }
    return -1;
}

double averagePower(const SignalSource &signal) {
    double size = signal.size();
    double average = 0.0;
    for (auto signalItem : signal) {
        average += signalItem * signalItem / size;
    }
    return average;
}

double SNR(const SignalSource &testSignal, const SignalSource &cleanSignal) {
    auto cleanSignalPower = averagePower(cleanSignal);
    auto noisePower = averagePower(signalDifference(testSignal, cleanSignal));
    return 10 * log10(cleanSignalPower/noisePower);
}

QString generateAudioFileName(const QString &str)
{
    return QString("%1audio%2.wav").arg(str).arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
}

}

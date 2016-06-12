#pragma once

#include <src/AudioSignal.h>

namespace AudioUtil {
using SignalSource = AudioSignal::SignalSource;

SignalSource makeWhiteNoise(const SignalSource &signal, double maxAmplitude, double density);
SignalSource amplitudeAverage(const SignalSource &signal, int step, bool absolute = false);
SignalSource signalDensity(const SignalSource &signal, bool positivePart = true, int max = Constants::audioUtilMaxDensityVal);

double maxAmplitude(const SignalSource &signal);

QString generateAudioFileName(const QString &str = "");
}



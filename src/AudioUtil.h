#pragma once

#include <src/AudioSignal.h>

namespace AudioUtil {
using SignalSource = AudioSignal::SignalSource;

SignalSource makeWhiteNoise(const SignalSource &signal, double maxAmplitude, double density);
SignalSource amplitudeAverage(const SignalSource &signal, int step, bool absolute = false);

QString generateAudioFileName(const QString &str = "");
}



#pragma once

#include <src/AudioSignal.h>

namespace AudioUtil {
using SignalSource = AudioSignal::SignalSource;

SignalSource makeWhiteNoise(const SignalSource &signal, double maxAmplitude = Constants::noiseDefaultMaxAmplitude, double density = Constants::noiseDefaultDensity);
SignalSource amplitudeAverage(const SignalSource &signal, int step, bool absolute = false);
SignalSource signalDensity(const SignalSource &signal, bool positivePart = true, int max = Constants::audioUtilMaxDensityVal);
SignalSource signalDifference(const SignalSource &first, const SignalSource &second);
SignalSource derivative(const SignalSource &source, int h, bool smooth);

double findMax(const SignalSource &signal);
double findMin(const SignalSource &signal);
int findIndexOfMoreThan(const SignalSource &signal, double val, int startIndex = 0);
int indexOfMin(const SignalSource &signal);
double SNR(const SignalSource &testSignal, const SignalSource &cleanSignal);

QString generateAudioFileName(const QString &str = "");
}



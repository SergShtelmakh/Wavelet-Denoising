#pragma once

#include <src/AudioSignal.h>

namespace AudioUtil {
using SignalSource = AudioSignal::SignalSource;

SignalSource makeWhiteNoise(SignalSource &signal, double maxAmplitude, double density);
}



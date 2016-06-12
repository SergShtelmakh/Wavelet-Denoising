#include "AudioUtil.h"

namespace {

int rand(int min, int max) {
    Q_ASSERT(max > min);
    return qrand() % ((max + 1) - min) + min;
}

bool genenerateRandomEvent(double probability)
{
    const int64_t maxRand = 1000;
    return rand(0, maxRand) < probability * maxRand;
}

}


AudioUtil::SignalSource AudioUtil::makeWhiteNoise(SignalSource &signal, double maxAmplitude, double density) {
    SignalSource result;
    for (auto sample : signal) {
        if (genenerateRandomEvent(density)) {
            sample = sample + rand(-maxAmplitude, maxAmplitude);
        }
        result << sample;
    }
    return result;
}





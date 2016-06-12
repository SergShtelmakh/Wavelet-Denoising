#pragma once

#include <QString>

#include <src/AudioSignal.h>
#include <src/Constants.h>

class ThresholdsManager
{
public:
    using SignalsSourceVector = AudioSignal::SignalsSourceVector;
    using SignalSource = AudioSignal::SignalSource;

    enum ThresholdType {
        Hard  = 0,
        Soft  = 1,
        CustomHard = 2,
        CustomSoft = 3,

        First = ThresholdType::Hard,
        Last  = ThresholdType::CustomSoft
    };

    static QList<QString> thresholdsNames();
    static ThresholdType fromString(const QString& str);

    ThresholdType thresholdType() const { return m_thresholdType; }
    void setThresholdType(const ThresholdType &thresholdType) { m_thresholdType = thresholdType; }

    SignalsSourceVector signalsVector() const { return m_signalsVector; }
    void setSignalsVector(const SignalsSourceVector &signalsVector) { m_signalsVector = signalsVector; }

    void makeThreshold(const QVector<double> &thresholdLevels);

    SignalsSourceVector thresholdedSignalsVector() const { return m_thresholdedSignalsVector; }

    void automaticThreshold(const ThresholdType &type);

private:
    SignalSource threshodedSignal(ThresholdType type,const SignalSource &signal, double threshod, double step, double overValue = Constants::thresholdsDefaultOverValue);
    ThresholdType m_thresholdType;

    SignalsSourceVector m_signalsVector;
    SignalsSourceVector m_thresholdedSignalsVector;
};

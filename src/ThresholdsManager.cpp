#include "ThresholdsManager.h"

#include <src/AudioUtil.h>

#include <QHash>

namespace {

using ThresholdType = ThresholdsManager::ThresholdType;
using ThresholdTypeHash = QHash<ThresholdType, QString>;

ThresholdTypeHash makeThresholdNameHash() {
    ThresholdTypeHash names;
    names[ThresholdType::Hard]  = "Hard";
    names[ThresholdType::Soft]  = "Soft";
    names[ThresholdType::CustomHard] = "Custom Hard";
    names[ThresholdType::CustomSoft] = "Custom Soft";
    return names;
}

static const auto thresholdsNameHash = makeThresholdNameHash();
}

QList<QString> ThresholdsManager::thresholdsNames()
{
    auto first = static_cast<int>(ThresholdType::First);
    auto last = static_cast<int>(ThresholdType::Last);
    QList<QString> names;
    for (auto i = first; i <= last; i++) {
        auto currentType = static_cast<ThresholdType>(i);
        names << toString(currentType);
    }
    return names;
}

ThresholdsManager::ThresholdType ThresholdsManager::fromString(const QString &str)
{
    return thresholdsNameHash.key(str, ThresholdType::Hard);
}

QString ThresholdsManager::toString(const ThresholdsManager::ThresholdType &type)
{
    return thresholdsNameHash.value(type, "");
}

ThresholdsManager::SignalSource ThresholdsManager::threshodedSignal(ThresholdsManager::ThresholdType type,
                                                    const SignalSource &signal,
                                                    double threshod,
                                                    double step,
                                                    double overValue)
{
    QVector<double> result;
    switch (type) {
    case ThresholdType::Hard: {
        for (auto signalItem : signal) {
            result.append((qAbs(signalItem) > threshod) ? signalItem : 0);
        }
        break;
    }
    case ThresholdType::Soft: {
        for (auto signalItem : signal) {
            result.append(qMax(0.0, 1.0 - (threshod/ qAbs(signalItem + 0.0001))) * signalItem);
        }
        break;
    }
    case ThresholdType::CustomHard: {
        for (auto signalItem : signal) {
            result.append((qAbs(signalItem) > threshod) ? signalItem : 0);
        }
        auto averageVector = AudioUtil::amplitudeAverage(signal, step, true);
        for (int i = 0; i < result.size(); i++) {
            if (averageVector[i] - threshod < overValue) {
                result[i] = 0;
            }
        }
        break;
    }

    case ThresholdType::CustomSoft: {
        for (auto signalItem : signal) {
            result.append(qMax(0.0, 1.0 - (threshod/ qAbs(signalItem + 0.0001))) * signalItem);
        }
        auto averageVector = AudioUtil::amplitudeAverage(signal, step, true);
        for (int i = 0; i < result.size(); i++) {
            if (averageVector[i] - threshod < overValue) {
                result[i] = 0;
            }
        }
        break;
    }
    default:
        qDebug("Wrong thresholds type");
        break;
    }

    return result;
}

void ThresholdsManager::makeThreshold(const QVector<double> &thresholdLevels)
{
    if (thresholdLevels.size() != m_signalsVector.size()) {
        qDebug("Wrong thresholds vector size");
        return;
    }

    m_thresholdedSignalsVector.clear();
    for (int i = 0; i < thresholdLevels.size(); ++i) {
        m_thresholdedSignalsVector << threshodedSignal(m_thresholdType,
                                                       m_signalsVector.at(i),
                                                       thresholdLevels[i],
                                                       Constants::thresholdsDefaultAverageCount,
                                                       Constants::thresholdsDefaultOverValue);
    }
}

void ThresholdsManager::automaticThreshold(const ThresholdsManager::ThresholdType &type)
{
    m_thresholdedSignalsVector.clear();

    for (int i = 0; i < m_signalsVector.size(); i++) {
        auto average = AudioUtil::amplitudeAverage(m_signalsVector.at(i), Constants::thresholdsDefaultAverageCount, true);
        auto density = AudioUtil::signalDensity(average);
        auto averageDensity = AudioUtil::amplitudeAverage(density, Constants::thresholdsDefaultAverageCount);
        auto derivative = AudioUtil::derivative(averageDensity, 100, true);
        // TODO fix thresholds level
        auto level = AudioUtil::indexOfMin(derivative);

        m_thresholdedSignalsVector << threshodedSignal(type,
                                                       m_signalsVector.at(i),
                                                       level,
                                                       Constants::thresholdsDefaultAverageCount,
                                                       Constants::thresholdsDefaultOverValue);

    }
}

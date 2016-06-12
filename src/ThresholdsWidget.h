#pragma once

#include <QWidget>

#include <src/AudioSignal.h>
#include <src/PlotManager.h>

namespace Ui { class ThresholdsWidget; }

class ThresholdsWidget : public QWidget
{
    Q_OBJECT
public:
    using SignalSource = AudioSignal::SignalSource;
    explicit ThresholdsWidget(QWidget *parent = 0);
    ~ThresholdsWidget();

    SignalSource signalSource() const;
    void setSignalSource(const SignalSource &signalSource);

    double maxThreshold() const;
    void setMaxThreshold(double maxThreshold);

    double threshold() const;
    void setThreshold(double threshold);

signals:
    void signalChanged(const SignalSource &signalSource);
    void maxThresholdChanged(double maxThreshold);
    void thresholdChanged(double threshold);

private slots:
    void on_vsThresholdsLevel_valueChanged(int value);
    void on_sbThresholdsLevel_valueChanged(int arg1);

private:
    void replotSignal();
    void replotThreshold();
    void replotAverage();
    void replotDensity();

    SignalSource m_signalSource;
    double m_maxThreshold;
    double m_threshold = 0.0;

    SignalSource m_signalDensity;
    SignalSource m_signalAverage;

    PlotManager::PlotData m_signalPlotData;
    PlotManager::PlotData m_densityPlotData;
    PlotManager::PlotData m_averagePlotData;

    Ui::ThresholdsWidget *ui;
};

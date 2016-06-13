#pragma once

#include <src/AudioSignal.h>
#include <src/AnalyzerWidget.h>

class QCustomPlot;

namespace PlotManager {

using SignalSource = AudioSignal::SignalSource;

struct PlotData {
    QVector<double> x;
    QVector<double> y;
    double minX = 0.0;
    double maxX = 0.0;
    double minY = 0.0;
    double maxY = 0.0;

    PlotData(QVector<double> x,  QVector<double> y, double minX = 0.0, double maxX = 0.0, double minY = 0.0, double maxY = 0.0)
        : x(x), y(y), minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}
    PlotData() {}
};

    PlotData createPlotData(const SignalSource &data, double minX, double maxX);
    PlotData createPlotData(const SignalSource &data);

    void plot(QCustomPlot* widget, const SignalSource &data, double minX, double maxX, bool interaction = true);
    void plot(QCustomPlot *widget, const AudioSignal &signal);
    void plot(QCustomPlot *widget, const SignalSource &signalSource);

    QCustomPlot* createPlot(const SignalSource &data, const QString &title = "");
    void createAudioPlot(const SignalSource &data);

    // TODO refactoring
    QCustomPlot *createPlot(const AnalyzerWidget::AnalyzerPlotData& data, double minX, double maxX);
}

#include "PlotManager.h"

#include <src/Constants.h>

#include <qcustomplot/qcustomplot.h>

PlotManager::PlotData PlotManager::createPlotData(const SignalSource &data, double minX, double maxX)
{
    PlotData result;
    if (data.size() == 0) {
        return result;
    }

    QVector<double> x(data.size());
    auto dx = (maxX - minX) / data.size();
    auto currentX = minX;
    auto minY = data.first();
    auto maxY = data.first();
    for (auto i = 0; i < data.size(); ++i) {
        x[i] = currentX;
        currentX += dx;
        minY = qMin(data[i], minY);
        maxY = qMax(data[i], maxY);
    }
    return { x, data, minX, maxX, minY, maxY };
}

PlotManager::PlotData PlotManager::createPlotData(const SignalSource &data)
{
    return createPlotData(data, 0, data.size());
}

void PlotManager::plot(QCustomPlot *widget, const SignalSource &data, double minX, double maxX, bool interaction)
{
    if (!widget || data.size() == 0) {
        return;
    }

    auto plotData = createPlotData(data, minX, maxX);

    widget->clearGraphs();
    widget->addGraph();
    widget->graph(0)->setData(plotData.x, plotData.y);

    widget->xAxis->setLabel("x");
    widget->yAxis->setLabel("y");

    widget->xAxis->setRange(plotData.minX, plotData.maxX);
    widget->yAxis->setRange(plotData.minY, plotData.maxY);
    if (interaction) {
        widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    }
    widget->replot();
}

void PlotManager::plot(QCustomPlot *widget, const AudioSignal &signal)
{
    plot(widget, signal.source(), 0, signal.audioLength());
}

QCustomPlot *PlotManager::createPlot(const SignalSource &data, const QString &title)
{
    QCustomPlot * pl = new QCustomPlot();
    plot(pl, data, 0, data.size());
    pl->plotLayout()->insertRow(0);
    pl->plotLayout()->addElement(0, 0, new QCPPlotTitle(pl, title));
    pl->setGeometry(Constants::plotDefaultGeometry);
    pl->show();
    return pl;
}


void PlotManager::plot(QCustomPlot *widget, const SignalSource &signalSource)
{
    plot(widget, signalSource, 0, signalSource.size());
}

void PlotManager::createAudioPlot(const SignalSource &data)
{
    QCustomPlot * pl = new QCustomPlot();
    plot(pl, data, 0, data.size() / (Constants::audioDefaultSampleFrequency / 1000));
    pl->setGeometry(Constants::plotDefaultGeometry);
    pl->show();
}

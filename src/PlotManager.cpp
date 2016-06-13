#include "PlotManager.h"

#include <src/Constants.h>

#include <qcustomplot/qcustomplot.h>

namespace {
    QPen getPenForThresholds(int i) {
        switch (i) {
        case 0:
        return QPen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
        case 1:
        return QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        case 2:
        return QPen(Qt::blue, 3, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
        case 3:
        return QPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
        }
        return QPen(Qt::green, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
}

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

QCustomPlot *PlotManager::createPlot(const AnalyzerWidget::AnalyzerPlotData &data, double minX, double maxX)
{
    QCustomPlot * pl = new QCustomPlot();

    int i = 0;

    double minY =  1000000.0;
    double maxY = -1000000.0;

    for (auto currentPlotData : data.data) {
         auto plotData = createPlotData(currentPlotData.second, minX, maxX);

         pl->addGraph();
         pl->graph()->setPen(getPenForThresholds(i));
         pl->graph()->setName(currentPlotData.first);
         pl->graph()->setData(plotData.x, plotData.y);
         minY = qMin(minY, plotData.minY);
         maxY = qMax(maxY, plotData.maxY);
         i++;
    }

    pl->xAxis->setLabel("x");
    pl->yAxis->setLabel("y");

    pl->xAxis->setRange(minX, maxX);
    pl->yAxis->setRange(minY, maxY);

    pl->legend->setVisible(true);
    pl->legend->setFont(QFont("Helvetica", 9));

    pl->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    pl->replot();
    pl->plotLayout()->insertRow(0);
    pl->plotLayout()->addElement(0, 0, new QCPPlotTitle(pl, data.waveletName));
    pl->setGeometry(Constants::plotDefaultGeometry);
    pl->show();
    return pl;
}

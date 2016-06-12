#include "ThresholdsWidget.h"
#include "ui_ThresholdsWidget.h"

#include <src/AudioUtil.h>
#include <src/PlotManager.h>
#include <src/Constants.h>

#include <qcustomplot/qcustomplot.h>

ThresholdsWidget::ThresholdsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThresholdsWidget)
{
    ui->setupUi(this);

    ui->signalPlot->addGraph(); // signal
    ui->signalPlot->addGraph(); // positive threshold
    ui->signalPlot->addGraph(); // negative threshold
    ui->signalPlot->graph(1)->setPen(Constants::thresholdsLevelPen);
    ui->signalPlot->graph(2)->setPen(Constants::thresholdsLevelPen);
    ui->signalPlot->xAxis->setLabel("Time");
    ui->signalPlot->yAxis->setLabel("Amplitude");

    ui->densityPlot->addGraph(); // density
    ui->densityPlot->addGraph(); // threshold
    ui->densityPlot->graph(1)->setPen(Constants::thresholdsLevelPen);
    ui->densityPlot->xAxis->setLabel("Amplitude");
    ui->densityPlot->yAxis->setLabel("N");

    ui->averagePlot->addGraph(); // average
    ui->averagePlot->addGraph(); // threshold
    ui->averagePlot->graph(1)->setPen(Constants::thresholdsLevelPen);
    ui->averagePlot->xAxis->setLabel("Time");
    ui->averagePlot->yAxis->setLabel("Amplitude");
}

ThresholdsWidget::~ThresholdsWidget()
{
    delete ui;

    m_signalSource.clear();
    m_signalDensity.clear();
    m_signalAverage.clear();

    m_signalPlotData.x.clear();
    m_signalPlotData.y.clear();
    m_densityPlotData.x.clear();
    m_densityPlotData.y.clear();
    m_averagePlotData.y.clear();
    m_averagePlotData.x.clear();
}

AudioSignal::SignalSource ThresholdsWidget::signalSource() const
{
    return m_signalSource;
}

void ThresholdsWidget::setSignalSource(const SignalSource &signal)
{
    if (m_signalSource == signal) {
        return;
    }

    m_signalSource = signal;
    emit signalChanged(m_signalSource);

    setMaxThreshold(AudioUtil::findMax(m_signalSource));
    m_signalAverage = AudioUtil::amplitudeAverage(m_signalSource, Constants::thresholdsDefaultAverageCount, true);
    m_signalDensity = AudioUtil::signalDensity(m_signalAverage);

    replotSignal();
    replotAverage();
    replotDensity();
    replotThreshold();
}

double ThresholdsWidget::maxThreshold() const
{
    return m_maxThreshold;
}

double ThresholdsWidget::threshold() const
{
    return m_threshold;
}

void ThresholdsWidget::setThreshold(double threshold)
{
    if (m_threshold == threshold) {
        return;
    }

    m_threshold = threshold;

    ui->vsThresholdsLevel->setValue(static_cast<int>(threshold));
    ui->sbThresholdsLevel->setValue(static_cast<int>(threshold));

    emit thresholdChanged(m_threshold);

    replotThreshold();
}

void ThresholdsWidget::replotSignal()
{
    if (m_signalSource.size() == 0) {
        return;
    }
    m_signalPlotData = PlotManager::createPlotData(m_signalSource, 0, m_signalSource.size());

    ui->signalPlot->graph(0)->setData(m_signalPlotData.x, m_signalPlotData.y);
    ui->signalPlot->xAxis->setRange(m_signalPlotData.minX, m_signalPlotData.maxX);
    ui->signalPlot->yAxis->setRange(m_signalPlotData.minY, m_signalPlotData.maxY);
    ui->signalPlot->replot();
}

void ThresholdsWidget::replotThreshold()
{
    QVector<double> thresholdVector;
    thresholdVector.fill(m_threshold, m_signalPlotData.x.size());
    ui->signalPlot->graph(1)->setData(m_signalPlotData.x, thresholdVector);
    thresholdVector.fill(-m_threshold, m_signalPlotData.x.size());
    ui->signalPlot->graph(2)->setData(m_signalPlotData.x, thresholdVector);
    ui->signalPlot->replot();

    QVector<double> thresholdVector2;
    thresholdVector2.fill(m_threshold, m_averagePlotData.x.size());
    ui->averagePlot->graph(1)->setData(m_averagePlotData.x, thresholdVector2);
    ui->averagePlot->replot();

    QVector<double> posT_x = { m_threshold, m_threshold + 1 };
    QVector<double> posT_y = { m_densityPlotData.minY, m_densityPlotData.maxY };
    ui->densityPlot->graph(1)->setData(posT_x, posT_y);
    ui->densityPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->densityPlot->replot();
}

void ThresholdsWidget::replotAverage()
{
    m_averagePlotData = PlotManager::createPlotData(m_signalAverage, 0, m_signalAverage.size());
    ui->averagePlot->graph(0)->setData(m_averagePlotData.x, m_averagePlotData.y);
    ui->averagePlot->xAxis->setRange(m_averagePlotData.minX, m_averagePlotData.maxX);
    ui->averagePlot->xAxis->setTickLabels(false);
    ui->averagePlot->yAxis->setRange(m_averagePlotData.minY, m_averagePlotData.maxY);
    ui->averagePlot->replot();
}

void ThresholdsWidget::replotDensity()
{
    m_densityPlotData = PlotManager::createPlotData(m_signalDensity);

    ui->densityPlot->graph(0)->setData(m_densityPlotData.x, m_densityPlotData.y);
    ui->densityPlot->xAxis->setRange(m_densityPlotData.minX, m_densityPlotData.maxX);
    ui->densityPlot->xAxis->setTickLabels(false);
    ui->densityPlot->yAxis->setRange(m_densityPlotData.minY, m_densityPlotData.maxY);
    ui->densityPlot->replot();
}

void ThresholdsWidget::setMaxThreshold(double maxThreshold)
{
    if (m_maxThreshold == maxThreshold) {
        return;
    }

    m_maxThreshold = maxThreshold;

    ui->sbThresholdsLevel->setMaximum(static_cast<int>(maxThreshold));
    ui->sbThresholdsLevel->setMinimum(0);
    ui->sbThresholdsLevel->setValue(0);

    ui->vsThresholdsLevel->setMaximum(static_cast<int>(maxThreshold));
    ui->vsThresholdsLevel->setMinimum(0);
    ui->vsThresholdsLevel->setValue(0);

    emit maxThresholdChanged(m_maxThreshold);
}

void ThresholdsWidget::on_vsThresholdsLevel_valueChanged(int value)
{
    setThreshold(value);
}

void ThresholdsWidget::on_sbThresholdsLevel_valueChanged(int arg1)
{
    setThreshold(arg1);
}

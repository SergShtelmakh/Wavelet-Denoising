#include "AnalyzerWidget.h"
#include "ui_AnalyzerWidget.h"

#include <src/PlotManager.h>

#include <QThread>

AnalyzerWidget::AnalyzerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnalyzerWidget)
    , m_pAnalyzerThread(new QThread)
    , m_analyzer(new SignalAnalyzer)
{
    ui->setupUi(this);

    connect(m_analyzer, &SignalAnalyzer::log, this, &AnalyzerWidget::log);
    connect(m_analyzer, &SignalAnalyzer::printResut, this, &AnalyzerWidget::printResut);
    connect(m_analyzer, &SignalAnalyzer::printCurrentResult, this, &AnalyzerWidget::printCurrentResult);
    connect(m_pAnalyzerThread, &QThread::started, m_analyzer, &SignalAnalyzer::start);
    connect(m_analyzer, &SignalAnalyzer::finished, m_pAnalyzerThread, &QThread::quit);

    m_analyzer->moveToThread(m_pAnalyzerThread);
}

AnalyzerWidget::~AnalyzerWidget()
{
    delete ui;
}

void AnalyzerWidget::setSignal(const AudioSignal::SignalSource &signal)
{
    m_analyzer->setSignal(signal);
}

void AnalyzerWidget::log(const QString &str)
{
    ui->pteLog->appendPlainText(str);
}

void AnalyzerWidget::printResut(const QString &waveletName, const QString &thresholdsName, const QVector<qreal> &data)
{
    if (m_plotData.waveletName != waveletName) {
        m_plotData.waveletName = waveletName;
        m_plotData.data.clear();
    }

    m_plotData.data.append(qMakePair(thresholdsName, data));
}

void AnalyzerWidget::printCurrentResult()
{
    auto minX = ui->sbMinNoise->value();
    auto maxX = ui->sbMinNoise->value() + ui->sbStep->value() * (m_plotData.data.first().second.size()-1);
    PlotManager::createPlot(m_plotData, minX, maxX);
}

void AnalyzerWidget::on_pbStart_clicked()
{
    if (m_pAnalyzerThread->isRunning()) {
        m_pAnalyzerThread->quit();
    }

    m_analyzer->setNoiseLevels(ui->sbMinNoise->value(), ui->spMaxNoise->value(), ui->sbStep->value());

    m_pAnalyzerThread->start();
}

void AnalyzerWidget::on_pbStop_clicked()
{
    m_pAnalyzerThread->quit();
}

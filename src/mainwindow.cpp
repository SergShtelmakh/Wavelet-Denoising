#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <src/AnalyzerWidget.h>
#include <src/AudioSignal.h>
#include <src/AudioPlayerWidget.h>
#include <src/AudioUtil.h>
#include <src/DenoisingWidget.h>
#include <src/DenoisingManager.h>
#include <src/PlotManager.h>

#include <QFileDialog>

namespace {
    const QString csTestFileName = "test.wav";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_fileName(csTestFileName)
    , m_inputSignal(new AudioSignal(csTestFileName))
    , m_processedSignal(new AudioSignal(m_inputSignal->source()))
    , m_denoisingWidget(new DenoisingWidget)
    , m_analyzerWidget(new AnalyzerWidget)
{
    ui->setupUi(this);
    updatePlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbShowSourceSignal_clicked()
{
    PlotManager::createPlot(m_inputSignal->source(), "Source signal");
}

void MainWindow::on_pbRevert_clicked()
{
    m_processedSignal.reset(new AudioSignal(m_inputSignal->source()));
    updatePlot();
}

void MainWindow::on_pbMakeWhiteNoise_clicked()
{
    m_processedSignal->makeWhiteNoise();
    updatePlot();
}

void MainWindow::on_pbManualDenoising_clicked()
{
    m_denoisingWidget->setNoisedSignal(*m_processedSignal.data());
    m_denoisingWidget->setClearSignal(*m_inputSignal.data());
    m_denoisingWidget->show();
}

void MainWindow::on_pbAutomaticDenoising_clicked()
{
    DenoisingManager manager;
    manager.setSignal(*m_processedSignal);
    manager.prepareToDenoising(Wavelet::Biorthogonal1_3, 5);
    manager.automaticDenoising(ThresholdsManager::CustomHard);
    m_processedSignal.reset(new AudioSignal(manager.denoisedSignal()));
    qDebug("SNR %f", AudioUtil::SNR(m_processedSignal->source(), m_inputSignal->source()));
    updatePlot();
}

void MainWindow::on_pbAutomaticDenoisingTest_clicked()
{
    m_analyzerWidget->setSignal(m_processedSignal->source());
    m_analyzerWidget->show();
}

void MainWindow::on_actionRecorder_triggered()
{
    // TODO
}

void MainWindow::on_actionOpen_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Audio"), "", tr("Wave Files (*.wav)"));
    if (QFileInfo(fileName).exists()) {
        m_inputSignal.reset(new AudioSignal(fileName));
        m_processedSignal.reset(new AudioSignal(m_inputSignal->source()));
        updatePlot();
        m_fileName = fileName;
    }
}

void MainWindow::on_actionSave_triggered()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("Save Audio"), "", tr("Wave Files (*.wav)"));
    if (!fileName.isEmpty()) {
        m_processedSignal->save(fileName);
    }
}

void MainWindow::updatePlot()
{
    ui->audioPlayerWidget->setSignal(*m_processedSignal.data());
}

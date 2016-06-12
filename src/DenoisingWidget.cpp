#include "DenoisingWidget.h"
#include "ui_DenoisingWidget.h"

#include <src/Constants.h>
#include <src/DenoisingManager.h>
#include <src/PlotManager.h>
// TODO
//#include <src/ThresholdsWidget.h>
#include <src/ThresholdsManager.h>
#include <src/wavelets/Wavelet.h>

#include <QScrollArea>
#include <QLayoutItem>

DenoisingWidget::DenoisingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DenoisingWidget)
    , m_denoisingManager(new DenoisingManager)
{
    ui->setupUi(this);

    ui->cbWaveletType->addItems(Wavelet::waveletFunctionsNames());
    ui->cbThresholdType->addItems(ThresholdsManager::thresholdsNames());
}

DenoisingWidget::~DenoisingWidget()
{
    delete ui;
}

void DenoisingWidget::setNoisedSignal(const AudioSignal &signal)
{
    m_noisedAudioSignal.reset(new AudioSignal(signal.source()));
    ui->inputSignalPlayerWidget->setSignal(m_noisedAudioSignal->source());

    m_denoisedAudioSignal.reset(new AudioSignal(signal.source()));
    ui->outputSignalPlayerWidget->setSignal(m_denoisedAudioSignal->source());
}

void DenoisingWidget::setClearSignal(const AudioSignal &signal)
{
    m_clearAudioSignal.reset(new AudioSignal(signal.source()));
}

QVector<double> DenoisingWidget::thresholdsData() const
{
    // TODO
    QVector<double> result;
//    for (auto w : m_widgets) {
//        result << w->threshold();
//    }
    return result;
}

void DenoisingWidget::cleanWidget()
{
    // TODO
//    while (!m_widgets.empty()) {
//        auto w = m_widgets.takeFirst();
//        delete w;
//    }
}

void DenoisingWidget::on_pbPrepare_clicked()
{
    // prepare widget
    cleanWidget();

    QLayout *layout;
    if (Constants::showThresholdsWidgetSeparately) {
        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setBackgroundRole(QPalette::Window);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);

        QWidget* wdg = new QWidget();
        wdg->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        wdg->setLayout(new QVBoxLayout(wdg));
        scrollArea->setWidget(wdg);
        scrollArea->show();

        layout = wdg->layout();
    } else {
        if (!ui->scrollAreaWidgetContents->layout()) {
            ui->scrollAreaWidgetContents->setLayout(new QVBoxLayout());
        }

        layout = ui->scrollAreaWidgetContents->layout();
    }

    // prepare signal (wavelet transform)
    m_denoisingManager->setSignal(*m_noisedAudioSignal.data());
    m_denoisingManager->prepareToDenoising(ui->cbWaveletType->currentText(), ui->sbLevel->value());
    PlotManager::plot(ui->inputTransformedSignalWidget, m_denoisingManager->transformedSignal());

    auto decomposition = m_denoisingManager->transformedDecomposition();
    m_itemsCount = decomposition.size();

    // initialize thresholds widgets

    // TODO
//    for (auto item : decomposition) {
//        auto wdg = new ThresholdsWidget(this);
//        m_widgets.push_back(wdg);
//        wdg->setSignalSource(item);
//        layout->addWidget(wdg);
//    }

//    Q_ASSERT(m_itemsCount == m_widgets.size());
}

void DenoisingWidget::on_pbProcess_clicked()
{
    m_denoisingManager->denoising(ui->cbThresholdType->currentText(), thresholdsData());

    PlotManager::plot(ui->outputTransformedSignalWidget, m_denoisingManager->thresholdedSignal());
    ui->outputSignalPlayerWidget->setSignal(m_denoisingManager->denoisedSignal());
}

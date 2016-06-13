#pragma once

#include <QWidget>

#include <src/AudioSignal.h>
#include <src/SignalAnalyzer.h>


namespace Ui { class AnalyzerWidget; }

class AnalyzerWidget : public QWidget
{
    Q_OBJECT
public:
    struct AnalyzerPlotData {
        QString waveletName;
        QVector<QPair<QString/*thresholds name*/, QVector<double>>> data;
    };

    explicit AnalyzerWidget(QWidget *parent = 0);
    ~AnalyzerWidget();

    void setSignal(const AudioSignal::SignalSource& signal);

private slots:
    void log(const QString &str);
    void printResut(const QString& waveletName, const QString& thresholdsName, const QVector<double>& data);
    void printCurrentResult();

    void on_pbStart_clicked();
    void on_pbStop_clicked();

private:
    Ui::AnalyzerWidget *ui;
    QThread *m_pAnalyzerThread;
    SignalAnalyzer *m_analyzer;
    AnalyzerPlotData m_plotData;
};

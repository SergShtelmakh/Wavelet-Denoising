#pragma once

#include <QWidget>

class AudioSignal;
class DenoisingManager;
//class ThresholdsWidget;

namespace Ui { class DenoisingWidget; }

class DenoisingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DenoisingWidget(QWidget *parent = 0);
    ~DenoisingWidget();

    void setNoisedSignal(const AudioSignal& signal);
    void setClearSignal(const AudioSignal& signal);

private slots:
    void on_pbPrepare_clicked();
    void on_pbProcess_clicked();

private:
    void cleanWidget();
    QVector<double> thresholdsData() const;

    Ui::DenoisingWidget *ui;
//    QVector<ThresholdsWidget *> m_widgets;
    QScopedPointer<DenoisingManager> m_denoisingManager;
    QScopedPointer<AudioSignal> m_noisedAudioSignal;
    QScopedPointer<AudioSignal> m_denoisedAudioSignal;

    QScopedPointer<AudioSignal> m_clearAudioSignal;
    int m_itemsCount = 0;
};

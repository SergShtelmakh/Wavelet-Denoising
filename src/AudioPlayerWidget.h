#pragma once

#include <QWidget>

class AudioSignal;
class QMediaPlayer;
class QTimer;

namespace Ui { class AudioPlayerWidget; }

class AudioPlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AudioPlayerWidget(QWidget *parent = 0);
    ~AudioPlayerWidget();

    void setFileName(const QString &fileName);
    void setSignal(const AudioSignal& signal);

private slots:
    void on_pbPlay_clicked();
    void on_pbPause_clicked();
    void on_pbStop_clicked();

    void updateButtonState();
    void updateTimeLinePlot();

private:
    struct Ranges { double minX; double maxX; double minY; double maxY; };

    void updateSignalPlot();

    Ui::AudioPlayerWidget* ui;
    QScopedPointer<AudioSignal> m_signal;
    QScopedPointer<QMediaPlayer> m_player;
    QScopedPointer<QTimer> m_playerStatusChecker;
    Ranges m_plotRanges;
};

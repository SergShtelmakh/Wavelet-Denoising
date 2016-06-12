#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>

namespace {
    const QString csTestFileName = "test.wav";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_fileName(csTestFileName)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbShowSourceSignal_clicked()
{

}

void MainWindow::on_pbRevert_clicked()
{

}

void MainWindow::on_pbMakeWhiteNoise_clicked()
{

}

void MainWindow::on_pbManualDenoising_clicked()
{

}

void MainWindow::on_pbAutomaticDenoising_clicked()
{

}

void MainWindow::on_pbAutomaticDenoisingTest_clicked()
{

}

void MainWindow::on_actionRecorder_triggered()
{

}

void MainWindow::on_actionOpen_triggered()
{

}

void MainWindow::on_actionSave_triggered()
{

}

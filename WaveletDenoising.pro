#-------------------------------------------------
#
# Project created by QtCreator 2016-06-12T14:00:35
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = WaveletDenoising
TEMPLATE = app

CONFIG   += C++11

SOURCES  += $$PWD/src/AnalyzerWidget.cpp \
            $$PWD/src/AudioPlayerWidget.cpp \
            $$PWD/src/AudioSignal.cpp \
            $$PWD/src/AudioUtil.cpp \
            $$PWD/src/DenoisingManager.cpp \
            $$PWD/src/DenoisingWidget.cpp \
            $$PWD/src/main.cpp \
            $$PWD/src/MainWindow.cpp \
            $$PWD/src/PlotManager.cpp \
            $$PWD/src/SignalAnalyzer.cpp \
            $$PWD/src/ThresholdsManager.cpp \
            $$PWD/src/ThresholdsWidget.cpp \
            $$PWD/src/wavelets/DiscretePeriodicWavelet.cpp \
            $$PWD/src/wavelets/Wavelet.cpp

HEADERS  += $$PWD/src/AnalyzerWidget.h \
            $$PWD/src/AudioPlayerWidget.h \
            $$PWD/src/AudioSignal.h \
            $$PWD/src/AudioUtil.h \
            $$PWD/src/Constants.h \
            $$PWD/src/DenoisingManager.h \
            $$PWD/src/DenoisingWidget.h \
            $$PWD/src/MainWindow.h \
            $$PWD/src/PlotManager.h \
            $$PWD/src/SignalAnalyzer.h \
            $$PWD/src/ThresholdsManager.h \
            $$PWD/src/ThresholdsWidget.h \
            $$PWD/src/wavelets/DiscretePeriodicWavelet.h \
            $$PWD/src/wavelets/Wavelet.h

FORMS    += $$PWD/src/AnalyzerWidget.ui \
            $$PWD/src/AudioPlayerWidget.ui \
            $$PWD/src/DenoisingWidget.ui \
            $$PWD/src/MainWindow.ui \
            $$PWD/src/ThresholdsWidget.ui

LIBS     += $$PWD/libs/libAquila.a \
            $$PWD/libs/wavelet2d.dll

CONFIG( debug, debug|release ) {
    # debug
    LIBS += $$PWD/libs/qcustomplotd1.dll
} else {
    # release
    LIBS += $$PWD/libs/qcustomplot1.dll
}

INCLUDEPATH += $$PWD/libs
DEPENDPATH  += $$PWD/libs

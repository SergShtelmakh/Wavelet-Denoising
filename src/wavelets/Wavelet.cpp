#include "Wavelet.h"

#include <QHash>

namespace Wavelet {

using WaveletHash = QHash<WaveletFunction, std::string>;

WaveletHash makeWaveletNameHash() {
    WaveletHash names;
    names[WaveletFunction::Haar] = "haar";

    names[WaveletFunction::Daubechies1] = "db1";
    names[WaveletFunction::Daubechies2] = "db2";
    names[WaveletFunction::Daubechies3] = "db3";
    names[WaveletFunction::Daubechies4] = "db4";
    names[WaveletFunction::Daubechies5] = "db5";
    names[WaveletFunction::Daubechies6] = "db6";
    names[WaveletFunction::Daubechies7] = "db7";
    names[WaveletFunction::Daubechies8] = "db8";
    names[WaveletFunction::Daubechies9] = "db9";
    names[WaveletFunction::Daubechies10] = "db10";
    names[WaveletFunction::Daubechies11] = "db11";
    names[WaveletFunction::Daubechies12] = "db12";
    names[WaveletFunction::Daubechies13] = "db13";
    names[WaveletFunction::Daubechies14] = "db14";
    names[WaveletFunction::Daubechies15] = "db15";

    names[WaveletFunction::Biorthogonal1_1] = "bior1.1";
    names[WaveletFunction::Biorthogonal1_3] = "bior1.3";
    names[WaveletFunction::Biorthogonal1_5] = "bior1.5";
    names[WaveletFunction::Biorthogonal2_2] = "bior2.2";
    names[WaveletFunction::Biorthogonal2_4] = "bior2.4";
    names[WaveletFunction::Biorthogonal2_6] = "bior2.6";
    names[WaveletFunction::Biorthogonal2_8] = "bior2.8";
    names[WaveletFunction::Biorthogonal3_1] = "bior3.1";
    names[WaveletFunction::Biorthogonal3_3] = "bior3.3";
    names[WaveletFunction::Biorthogonal3_5] = "bior3.5";
    names[WaveletFunction::Biorthogonal3_7] = "bior3.7";
    names[WaveletFunction::Biorthogonal3_9] = "bior3.9";
    names[WaveletFunction::Biorthogonal4_4] = "bior4.4";
    names[WaveletFunction::Biorthogonal5_5] = "bior5.5";
    names[WaveletFunction::Biorthogonal6_8] = "bior6.8";

    names[WaveletFunction::Coiflet1] = "coif1";
    names[WaveletFunction::Coiflet2] = "coif2";
    names[WaveletFunction::Coiflet3] = "coif3";
    names[WaveletFunction::Coiflet4] = "coif4";
    names[WaveletFunction::Coiflet5] = "coif5";

    names[WaveletFunction::Symmlet1] = "sym1";
    names[WaveletFunction::Symmlet2] = "sym2";
    names[WaveletFunction::Symmlet3] = "sym3";
    names[WaveletFunction::Symmlet4] = "sym4";
    names[WaveletFunction::Symmlet5] = "sym5";
    names[WaveletFunction::Symmlet6] = "sym6";
    names[WaveletFunction::Symmlet7] = "sym7";
    names[WaveletFunction::Symmlet8] = "sym8";
    names[WaveletFunction::Symmlet9] = "sym9";
    names[WaveletFunction::Symmlet10] = "sym10";

    return names;
}

static const auto waveletHash = makeWaveletNameHash();

QList<QString> waveletFunctionsNames()
{
    auto first = static_cast<int>(WaveletFunction::First);
    auto last = static_cast<int>(WaveletFunction::Last);
    QList<QString> names;
    for (auto i = first; i <= last; i++) {
        auto currentType = static_cast<WaveletFunction>(i);
        names << toString(currentType);
    }
    return names;
}

std::string toStdString(WaveletFunction function)
{
    return waveletHash.value(function, "");
}

QString toString(WaveletFunction function)
{
    return QString::fromStdString(waveletHash.value(function, ""));
}

WaveletFunction fromString(const QString &str)
{
    return waveletHash.key(str.toStdString(), Wavelet::Haar);
}

}

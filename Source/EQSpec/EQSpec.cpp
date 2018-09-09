/*
  ==============================================================================

    EQSpec.cpp
    Created: 7 Sep 2017 9:26:49pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "EQSpec.h"

#include <algorithm>
#include <array>
#include <cmath>

constexpr std::array<double, 11> EQ_11_BAND_ISO_FREQS {
    16.0,
    31.5,
    63.0,
    125.0,
    250.0,
    500.0,
    1000.0,
    2000.0,
    4000.0,
    8000.0,
    16000.0
};

constexpr std::array<double, 33> EQ_33_BAND_ISO_FREQS {
    12.5,
    16.0,
    20.0,
    25.0,
    31.5,
    40.0,
    50.0,
    63.0,
    80.0,
    100.0,
    125.0,
    160.0,
    200.0,
    250.0,
    315.0,
    400.0,
    500.0,
    630.0,
    800.0,
    1000.0,
    1250.0,
    1600.0,
    2000.0, 
    2500.0,
    3150.0,
    4000.0,
    5000.0,
    6300.0,
    8000.0,
    10000.0,
    12500.0,
    16000.0,
    20000.0
};

constexpr std::array<double, 35> EQ_35_BAND_ISO_FREQS {
    10.0,
    16.0,
    20.0,
    25.0,
    31.5,
    40.0,
    50.0,
    63.0,
    80.0,
    100.0,
    125.0,
    160.0,
    200.0,
    250.0,
    315.0,
    400.0,
    500.0,
    630.0,
    800.0,
    1000.0,
    1250.0,
    1600.0,
    2000.0,
    2500.0,
    3150.0,
    4000.0,
    5000.0,
    6300.0,
    8000.0,
    10000.0,
    12500.0,
    16000.0,
    20000.0,
    25000.0
};

EQSpec::EQSpec(EQType type) : type(type) {
    
}

void EQSpec::prepareToPlay (double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    
    IIRCoefficients temp = IIRCoefficients::makeBandPass(44100.0, 10000, 1.0);
    
    bands.clear();
    int eqtype = static_cast<int>(type);
    int denom = eqtype > 0 ? (3 << (eqtype - 1)) : 1;
    float fraction = 1.0f / denom;
    
    constexpr double referencefreq = 440.0;
    if (referencefreq >= sampleRate) {
        // odd samplerate, maybe zero. Let's skip making bands anyway
        return;
    }
    int numhighbands = static_cast<int>(denom * std::log2(std::min(sampleRate, 44100.0) * 0.5/referencefreq));
    int numlowbands = numhighbands;
    int numbands = numlowbands + numhighbands + 1;
    int referenceband = numlowbands;
    bands.resize(numbands);
    const auto q = Q(1.0 * fraction);
    const double factor = pow(2.0, fraction);
    //int referenceband;
    //if (numbands == 12) {
    //    referenceband = 6;
    //} else if (numbands == 36) {
    //    referenceband = 19;
    //} else {
    //    referenceband = ((numbands >> 1) + (denom >> 1) - 1);
    //} //6 : ((numbands >> 1) + (denom >> 1) - 1);
    std::list<EQBand> dynbands;
    dynbands.emplace_back(EQBand({440.0, IIRCoefficients::makeBandPass(sampleRate, 440.0, q) }));
    int generatedbands = 1;
    // generate upper bands
    while(generatedbands < numhighbands + 1) {
        const auto freq = dynbands.back().frequency * factor;
        if (freq >= std::min(sampleRate, 44100.0) * 0.5) { // hard-coded against max 44.1 kHz to be consistent across configurations
            // pop the last one, we'll create a high pass filter below
            //dynbands.pop_back();
            break;
        }
        dynbands.emplace_back(EQBand({freq, IIRCoefficients::makeBandPass(sampleRate, freq, q) }));
        ++generatedbands;
    }
    // generate lower bands
    while(generatedbands < numbands) {
        const auto freq = dynbands.front().frequency / factor;
        dynbands.emplace_front(EQBand({freq, IIRCoefficients::makeBandPass(sampleRate, freq, q) }));
        ++generatedbands;
    }
    // generate a lowest low pass band
//    {
//        const auto freq = dynbands.front().frequency / factor;
//        dynbands.emplace_front(EQBand({freq, IIRCoefficients::makeLowPass(sampleRate, freq, q) }));
//    }
    // replace the last one with a high pass filter and break
//    {
//        const auto freq = dynbands.back().frequency * factor;
//        dynbands.emplace_back(EQBand({freq, IIRCoefficients::makeHighPass(sampleRate, freq, q) }));
//    }
    size_t i = 0;
    for (const auto& band: dynbands) {
        bands[i++] = band;
    }
    /*
    for(int band = referenceband - 1; band > 0; --band) {
        const auto freq = bands[band + 1].frequency / factor;
        bands[band] = EQBand({freq, IIRCoefficients::makeBandPass(sampleRate, freq, q) });
    }
    for(int band = referenceband + 1; band < numbands - 1; ++band) {
        const auto freq = bands[band - 1].frequency * factor;
        bands[band] = EQBand({freq, IIRCoefficients::makeBandPass(sampleRate, freq, q) });
    }
    {
        const auto freq = bands[1].frequency / factor;
        bands[0] = EQBand({freq, IIRCoefficients::makeLowPass(sampleRate, freq, q) });
    }
    {
        const auto freq = bands[numbands - 2].frequency * factor;
        bands[numbands - 1] = EQBand({freq, IIRCoefficients::makeHighPass(sampleRate, freq, q) });
    }*/
}

size_t EQSpec::numbands() const {
    return bands.size();
}

const EQSpec::EQBand& EQSpec::getband(size_t band) const {
    return bands[band];
}

double EQSpec::Q(double bw) {
    return sqrt(pow(2.0, bw)) / (pow(2.0, bw) - 1.0);
}

EQSpec::EQType EQSpec::eqtype() const {
    return type;
}

double EQSpec::samplerate() const {
    return sampleRate;
}

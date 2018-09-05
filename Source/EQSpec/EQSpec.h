/*
  ==============================================================================

    EQSpec.h
    Created: 7 Sep 2017 9:26:49pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include <list>

class EQSpec {
public:
    enum class EQType {
        EQ_1_1_BAND,
        EQ_1_3_BAND,
        EQ_1_6_BAND,
        EQ_1_12_BAND,
        EQ_1_24_BAND,
        EQ_1_48_BAND,
        EQ_1_96_BAND,
        EQ_1_192_BAND
    };
    
    struct EQBand {
        double frequency;
        IIRCoefficients coefficients;
    };
    
    EQSpec(EQType type = EQType::EQ_1_1_BAND);
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    
    size_t numbands() const;
    const EQBand& getband(size_t band) const;
    static double Q(double bw);
    EQType eqtype() const;
    double samplerate() const;
private:
    EQType type;
    std::vector<EQBand> bands;
    double sampleRate;
};

/*
  ==============================================================================

    EQReferenceProcessor.h
    Created: 7 Sep 2017 9:09:28pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "../EQProcessor/EQProcessor.h"
#include "../EQSpec/EQSpec.h"

#include <memory>
#include <vector>

class EQReferenceProcessor : public EQProcessor {
public:
    EQReferenceProcessor(EQSpec spec, int numchannels, int rmswindow);
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void processSamples (float* samples[], const int numSamples) noexcept;
    void setRms(float ms) override;
    void rms(std::vector<float>& values, std::vector<float>& gradient) override;
    int numbands() const override;
    void setEqType(EQSpec::EQType type) override;
    EQSpec::EQType eqtype() const override;
    EQSpec eqspec() const override;
private:
    SpinLock filterlock;
    EQSpec spec;
    
    struct EQBand {
        std::shared_ptr<IIRFilter> filter;
        std::shared_ptr<AudioSampleBuffer> buffer;
        double gain;
        double rms;
    };
    
    std::vector<std::vector<EQBand>> filters;
    std::vector<float> rmslevels;
    double sampleRate;
    int samplesPerBlock;
    int numchannels;
    int rmswindow;
};

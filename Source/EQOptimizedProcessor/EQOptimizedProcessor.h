/*
  ==============================================================================

    EQOptimizedProcessor.h
    Created: 22 Sep 2017 9:13:23pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "../EQProcessor/EQProcessor.h"
#include "../EQSpec/EQSpec.h"
#include "../PluginProcessor.h"

#include <memory>
#include <vector>

class WideIIRFilter;

class EQOptimizedProcessor : public EQProcessor {
public:
    EQOptimizedProcessor(TheSeekerAudioProcessor& processor, EQSpec spec, int numchannels);
    ~EQOptimizedProcessor();
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void processSamples (float* samples[], const int numSamples) noexcept;
    void setRms(float ms) override;
    void rms(std::vector<float>& values, std::vector<float>& gradient) override;
    int numbands() const override;
    void setEqType(EQSpec::EQType type) override;
    EQSpec::EQType eqtype() const override;
    EQSpec eqspec() const override;
    int level() const override;
    void level(int value) override;
private:
    SpinLock filterlock;
    EQSpec spec;
    int _level = 0;

	TheSeekerAudioProcessor& processor;

    struct EQBand {
        std::shared_ptr<AudioSampleBuffer> buffer;
        double gain;
        double rms;
    };

    std::unique_ptr<WideIIRFilter> filter;
    std::vector<std::vector<EQBand>> bands;
    std::vector<double> rmslevels;
    double sampleRate;
    int samplesPerBlock;
    int numchannels;
    int rmswindow;
    float rmstime = 100.0f;
};

/*
  ==============================================================================

    EQOptimizedProcessor.cpp
    Created: 22 Sep 2017 9:13:23pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "EQOptimizedProcessor.h"

#include "../WideIIRFilter/WideIIRFilter.h"

//#include <Eigen/Dense>

EQOptimizedProcessor::EQOptimizedProcessor(TheSeekerAudioProcessor& processor, EQSpec spec, int numchannels) : 
processor(processor), spec(spec), numchannels(numchannels) 
{
    
}

EQOptimizedProcessor::~EQOptimizedProcessor() = default;

void EQOptimizedProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    //const SpinLock::ScopedLockType sl (filterlock);
    this->sampleRate = sampleRate;
    this->samplesPerBlock = samplesPerBlock;
    bands.clear();
    spec.prepareToPlay(sampleRate, samplesPerBlock);
    filter = WideIIRFilter::make(spec);
	filter->setRms(processor.rms());
    rmslevels.resize(spec.numbands());
    std::vector<double> gains;
    //gains.push_back(0.5 * 0.707 + 1.0 / (spec.getband(0).frequency));
    //for (size_t filter = 1; filter < spec.numbands() - 1; ++filter) {
    for (size_t filter = 0; filter < spec.numbands(); ++filter) {
        gains.push_back(0.707 ); //Random::getSystemRandom().nextDouble());
    }
    //gains.push_back(1.0 / (0.5 * sampleRate - spec.getband(spec.numbands() - 1).frequency));
    for (int channel = 0; channel < numchannels; ++channel) {
        bands.push_back(decltype(bands)::value_type());
        for (size_t band = 0; band < spec.numbands(); ++band) {
            EQBand eqband = { std::make_shared<AudioSampleBuffer>(1, samplesPerBlock), 1.0};
            eqband.gain = gains[band];
            //eqband->filter = std::make_unique<IIRFilter>();
            filter->setCoefficients(band, spec.getband(band).coefficients);
            bands[channel].push_back(eqband);
            //eqband->buffer = std::make_unique<AudioSampleBuffer>(numchannels, samplesPerBlock);
            //filters[channel].push_back({ std::make_unique<IIRFilter>(spec.getband(filter).coefficients), std::make_unique<AudioSampleBuffer>(numchannels, samplesPerBlock) });
        }
    }
    
}

void EQOptimizedProcessor::processSamples(float* samples[], const int numSamples) noexcept {
    const SpinLock::ScopedLockType sl (filterlock);
	filter->processSamples(samples, numSamples, numchannels);
}

void EQOptimizedProcessor::setRms(float ms) {
    // rmstime = ms; // Not necessary? filter reads from processor when it's created
    if(filter) {
        filter->setRms(ms);
    }
}

void EQOptimizedProcessor::rms(std::vector<float>& values, std::vector<float>& gradient) {
    const SpinLock::ScopedLockType sl (filterlock);
    if (filter) {
        filter->rms(values, gradient);
    }
}

int EQOptimizedProcessor::numbands() const {
    return spec.numbands();
}

void EQOptimizedProcessor::setEqType(EQSpec::EQType type) {
    {
        const SpinLock::ScopedLockType sl (filterlock);
        // reset eqprocessor
        spec = EQSpec(type);
    
    prepareToPlay(sampleRate, samplesPerBlock);
    }
}

EQSpec::EQType EQOptimizedProcessor::eqtype() const {
    const SpinLock::ScopedLockType sl (filterlock);
    return spec.eqtype();
}

EQSpec EQOptimizedProcessor::eqspec() const {
    const SpinLock::ScopedLockType sl (filterlock);
    return spec;
}

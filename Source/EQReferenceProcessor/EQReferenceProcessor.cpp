/*
  ==============================================================================

    EQReferenceProcessor.cpp
    Created: 7 Sep 2017 9:09:28pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "EQReferenceProcessor.h"

//#include <Eigen/Dense>

EQReferenceProcessor::EQReferenceProcessor(EQSpec spec, int numchannels, int rmswindow) : spec(spec), numchannels(numchannels), rmswindow(rmswindow) {
    
}

void EQReferenceProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    this->samplesPerBlock = samplesPerBlock;
    filters.clear();
    spec.prepareToPlay(sampleRate, samplesPerBlock);
    rmslevels.resize(spec.numbands());
    std::vector<double> gains;
    for (size_t filter = 0; filter < spec.numbands(); ++filter) {
        gains.push_back(0.707 ); //Random::getSystemRandom().nextDouble());
    }
    for (int channel = 0; channel < numchannels; ++channel) {
        filters.push_back(decltype(filters)::value_type());
        for (size_t filter = 0; filter < spec.numbands(); ++filter) {
            EQBand eqband = { std::make_shared<IIRFilter>(), std::make_shared<AudioSampleBuffer>(1, samplesPerBlock), 1.0};
            eqband.gain = gains[filter];
            //eqband->filter = std::make_unique<IIRFilter>();
            eqband.filter->setCoefficients(spec.getband(filter).coefficients);
            filters[channel].push_back(eqband);
            //eqband->buffer = std::make_unique<AudioSampleBuffer>(numchannels, samplesPerBlock);
            //filters[channel].push_back({ std::make_unique<IIRFilter>(spec.getband(filter).coefficients), std::make_unique<AudioSampleBuffer>(numchannels, samplesPerBlock) });
        }
    }
    
}

void EQReferenceProcessor::processSamples(float* samples[], const int numSamples) noexcept {
    for (size_t i = 0; i < numchannels; ++i) {
    //for (auto& channel : filters) {
        auto& channel = filters[i];
        for (auto& band : channel) {
			// TODO: this is removed temporarily, lost changes in juce!!
            //band.filter->processSamples(samples[i], numSamples, band.buffer->getWritePointer(0));
            band.rms = band.buffer->getRMSLevel(0, numSamples - rmswindow, rmswindow);
        }
        FloatVectorOperations::clear(samples[i], numSamples);
    }
    // copy back data
    double weight = 1.0 / numchannels; // unnecessary to recalculate this
    for (size_t j = 0; j < spec.numbands(); ++j) {
        double rms = 0.0;
        for (size_t i = 0; i < numchannels; ++i) {
            auto& channel = filters[i];
            auto& band = channel[j];
            FloatVectorOperations::addWithMultiply(samples[i], band.buffer->getReadPointer(0), band.gain, numSamples);
            rms += band.buffer->getRMSLevel(0, 0, numSamples) * weight;
        }
        rmslevels[j] = rms;
    }
}

void EQReferenceProcessor::setRms(float ms) {
    
}

void EQReferenceProcessor::rms(std::vector<float>& values, std::vector<float>& gradient) {
    //return &this->rmslevels[0];
}

int EQReferenceProcessor::numbands() const {
    return spec.numbands();
}

void EQReferenceProcessor::setEqType(EQSpec::EQType type) {
    const SpinLock::ScopedLockType sl (filterlock);
    // reset eqprocessor
    
}

EQSpec::EQType EQReferenceProcessor::eqtype() const {
    const SpinLock::ScopedLockType sl (filterlock);
    // return eqtype
	return spec.eqtype();
}

EQSpec EQReferenceProcessor::eqspec() const {
    const SpinLock::ScopedLockType sl (filterlock);
    return spec;
}

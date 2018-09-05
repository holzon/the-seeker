/*
  ==============================================================================

    WideIIRFilter.h
    Created: 22 Sep 2017 9:11:54pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include <memory>

#include "../EQSpec/EQSpec.h"

class WideIIRFilter {
public:
    virtual ~WideIIRFilter() = default;
        //==============================================================================
    /** Clears the filter so that any incoming data passes through unchanged. */
    virtual void makeInactive() noexcept = 0;
    
    /** Applies a set of coefficients to a band of this filter. */
    virtual void setCoefficients (size_t index, const IIRCoefficients& newCoefficients) noexcept = 0;

    /** Applies gain to a band of this filter. */
    virtual void setGain(size_t index, float value) noexcept = 0;
    
    virtual void rms(std::vector<float>& values, std::vector<float>& gradient) = 0;
    virtual void setRms(float ms) = 0;
    
    //==============================================================================
    /** Resets the filter's processing pipeline, ready to start a new stream of data.
     
     Note that this clears the processing state, but the type of filter and
     its coefficients aren't changed. To put a filter into an inactive state, use
     the makeInactive() method.
     */
    virtual void reset() noexcept = 0;
    
    /** Performs the filter operation on the given set of samples. */
    virtual void processSamples (float* samples, int numSamples, int numChannels) noexcept = 0;

	virtual const EQSpec& eqspec() = 0;
    
    static std::unique_ptr<WideIIRFilter> make(const EQSpec& eqspec);
};

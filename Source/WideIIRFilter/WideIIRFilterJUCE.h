/*
  ==============================================================================

    WideIIRFilterJUCE.h
    Created: 24 Sep 2017 8:43:10pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "WideIIRFilter.h"

#include <array>
#include <vector>

#include <Eigen/Dense>

template<size_t N>
class WideIIRFilterJUCE : public WideIIRFilter
{
public:
    //==============================================================================
    /** Creates a filter.
     
     Initially the filter is inactive, so will have no effect on samples that
     you process with it. Use the setCoefficients() method to turn it into the
     type of filter needed.
     */
    WideIIRFilterJUCE() noexcept;
    
    /** Destructor. */
    ~WideIIRFilterJUCE() noexcept;
    
    //==============================================================================
    /** Clears the filter so that any incoming data passes through unchanged. */
    void makeInactive() noexcept override;
    
    /** Applies a set of coefficients to a band of this filter. */
    void setCoefficients (size_t index, const IIRCoefficients& newCoefficients) noexcept override;

    /** Applies gain to a band of this filter. */
    void setGain(size_t index, float value) noexcept override;
    
    //==============================================================================
    /** Resets the filter's processing pipeline, ready to start a new stream of data.
     
     Note that this clears the processing state, but the type of filter and
     its coefficients aren't changed. To put a filter into an inactive state, use
     the makeInactive() method.
     */
    void reset() noexcept override;
    
    /** Performs the filter operation on the given set of samples. */
    void processSamples (float* samples[], int numSamples, int numChannels) noexcept override;

	const EQSpec& eqspec() override;

    void resize(int numSamples, int numChannels);
       
protected:
    //==============================================================================
    SpinLock processLock;
	EQSpec eqspec_;
    struct ChannelState {
        ChannelState(){
            v.setZero();
        }
        Eigen::Array<float, N, 2> v, lv;
        //std::vector<Eigen::Array<float, N, Eigen::Dynamic>> buffer;
    };
    std::vector<ChannelState> channelState;
    Eigen::Array<float, N, 5> coefficients;
    Eigen::Array<float, N, 1> gains;
    Eigen::Array<float, N, 1> rms;
    // these are temporary buffers
    Eigen::Array<float, N, 1> out, sampleMean, totalSampleMean;
    size_t numChannels;
    size_t numSamples;
    float channelWeight;

    bool active;
    
    WideIIRFilterJUCE& operator= (const WideIIRFilterJUCE&) { }
    JUCE_LEAK_DETECTOR (WideIIRFilterJUCE)
};

//==============================================================================
template <size_t N>
WideIIRFilterJUCE<N>::WideIIRFilterJUCE() noexcept
: active (false), numChannels(0), numSamples(0), channelWeight(0.0f)
{
    gains = 1.0f;
}

template <size_t N>
WideIIRFilterJUCE<N>::~WideIIRFilterJUCE() noexcept
{
}

//==============================================================================
template <size_t N>
void WideIIRFilterJUCE<N>::makeInactive() noexcept
{
    const SpinLock::ScopedLockType sl (processLock);
    active = false;
}

template <size_t N>
void WideIIRFilterJUCE<N>::setCoefficients (size_t index, const IIRCoefficients& newCoefficients) noexcept
{
    const SpinLock::ScopedLockType sl (processLock);
    // TODO: make this nicer
    size_t offset = index * 5;
    coefficients(index, 0) = newCoefficients.coefficients[0];
    coefficients(index, 1) = newCoefficients.coefficients[1];
    coefficients(index, 2) = newCoefficients.coefficients[2];
    coefficients(index, 3) = newCoefficients.coefficients[3];
    coefficients(index, 4) = newCoefficients.coefficients[4];
    active = true;
}

template <size_t N>
void WideIIRFilterJUCE<N>::setGain(size_t index, float value) noexcept {
    gains[index] = value;
}

//==============================================================================
template <size_t N>
void WideIIRFilterJUCE<N>::reset() noexcept
{
    const SpinLock::ScopedLockType sl (processLock);
    // TODO: do I even need reset? Does nothing right now
}
//
//#if JUCE_INTEL
//#define JUCE_SNAP_TO_ZERO(n)    if (! (n < -1.0e-8 || n > 1.0e-8)) n = 0;
//#else
//#define JUCE_SNAP_TO_ZERO(n)
//#endif

template <size_t N>
void WideIIRFilterJUCE<N>::resize(int numSamples, int numChannels) {
    if (this->numChannels != numChannels) {
        channelState.resize(numChannels);
    }
    if (this->numSamples != numSamples || this->numChannels != numChannels) {
        for (int channel = 0; channel < numChannels; ++channel) {
          //  channelState[channel].buffer.resize(N, numSamples);
        }
        this->numChannels = numChannels;
        this->numSamples = numSamples;
        channelWeight = 1.0f / numChannels;
    }
}

template <size_t N>
void WideIIRFilterJUCE<N>::processSamples(float* samples[], const int numSamples, const int numChannels) noexcept
{
    const SpinLock::ScopedLockType sl (processLock);

    resize(numSamples, numChannels);

    if (active)
    {
        //Eigen::Map<Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Aligned16> inputsamples(samples, numChannels, numSamples);

		using arraymap = Eigen::Map<Eigen::Array<float, Eigen::Dynamic, 1>, Eigen::Unaligned>;
		std::vector<arraymap> inputsamples;
		for (int i = 0; i < numChannels; ++i)
		{
			inputsamples.emplace_back(samples[i], numSamples);
		}

        // TODO: not optimal do go over channels like this, but necessary for rms?
        totalSampleMean.setZero();
        for (int i = 0; i < numSamples; ++i)
        {
            sampleMean.setZero();
            for (int channel = 0; channel < numChannels; ++channel) {
                auto& channelState = this->channelState[channel];

                float& in = inputsamples[0](i);
                out = coefficients.col(0) * in + channelState.v.col(0);

                channelState.v.col(0) = coefficients.col(1) * in - coefficients.col(3) * out + channelState.v.col(1);
                channelState.v.col(1) = coefficients.col(2) * in - coefficients.col(4) * out;
                // multiply all bands outvalues with gains
                in = out.matrix().transpose() * gains.matrix();
                sampleMean += out * channelWeight;
            }
            totalSampleMean += sampleMean * sampleMean;
        }
        rms = (totalSampleMean * 1.0f / numSamples).sqrt();

//        const float& c0 = coefficients[0];
//        const float& c1 = coefficients[1];
//        const float& c2 = coefficients[2];
//        const float& c3 = coefficients[3];
//        const float& c4 = coefficients[4];
//        float lv1 = v1, lv2 = v2;
//
//        for (int i = 0; i < numSamples; ++i)
//        {
//            const float in = samples[i];
//            const float out = c0 * in + lv1;
//            samples[i] = out;
//
//            lv1 = c1 * in - c3 * out + lv2;
//            lv2 = c2 * in - c4 * out;
//        }
//
//        JUCE_SNAP_TO_ZERO (lv1);  v1 = lv1;
//        JUCE_SNAP_TO_ZERO (lv2);  v2 = lv2;
    }
}

template <size_t N>
const EQSpec& WideIIRFilterJUCE<N>::eqspec() {
	return eqspec_;
}

#undef JUCE_SNAP_TO_ZERO
/*
  ==============================================================================

    WideIIRFilterEigen.h
    Created: 23 Sep 2017 11:43:30am
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#define JUCE_USE_SSE_INTRINSICS 1

#include "WideIIRFilter.h"
#include "../Utils/MeanIntegrator.h"

#include <array>
#include <functional>
#include <vector>
#include <cmath>
#include <cstring>

#include <Eigen/Dense>

template<size_t N>
class WideIIRFilterEigen : public WideIIRFilter
{
public:
    //==============================================================================
    /** Creates a filter.
     
     Initially the filter is inactive, so will have no effect on samples that
     you process with it. Use the setCoefficients() method to turn it into the
     type of filter needed.
     */
    WideIIRFilterEigen(const EQSpec& eqspec) noexcept;
    
    /** Destructor. */
    ~WideIIRFilterEigen() noexcept;
    
    //==============================================================================
    /** Clears the filter so that any incoming data passes through unchanged. */
    void makeInactive() noexcept override;
    
    /** Applies a set of coefficients to a band of this filter. */
    void setCoefficients (size_t index, const IIRCoefficients& newCoefficients) noexcept override;

    /** Applies gain to a band of this filter. */
    void setGain(size_t index, float value) noexcept override;
    
    void rms(std::vector<float>& values, std::vector<float>& gradient) override;
    
    void setRms(float ms) override;
    
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
			//lv.setZero();
        }
		Eigen::Array<float, N, 2> v; //, lv;
        //std::vector<Eigen::Array<float, N, Eigen::Dynamic>> buffer;
    };
    std::vector<ChannelState> channelState;
    Eigen::Array<float, N, 5> coefficients;
    Eigen::Array<float, N, 1> gains;
    Eigen::Array<float, N, 1> rmsarray;
    Eigen::Array<float, N, 1> rmsarrayold; // This is not really necessary, two buffers are enough
    Eigen::Array<float, N, 1> rmsgradient;
    // these are temporary buffers
    Eigen::Array<float, N, 1> out, sampleMean, totalSampleMean;

	// Integration buffers, for longer rms times
	MeanIntegrator<float, N> rmsmean;
	MeanIntegrator<float, N> rmsgradientmean;

    size_t numChannels;
    size_t numSamples;
    float channelWeight;
    float rmsscale;
    int rmsscalesamples;
    double samplerate;
    float rmstime = 10.0f; // ms
	float rmsintegrationtime = 10.0f; // ms
    size_t numrmssamples = 0;

    bool active;
    
    WideIIRFilterEigen& operator= (const WideIIRFilterEigen&) { }
    JUCE_LEAK_DETECTOR (WideIIRFilterEigen)
};

//==============================================================================
template <size_t N>
WideIIRFilterEigen<N>::WideIIRFilterEigen(const EQSpec& eqspec) noexcept
: active(false), numChannels(0), numSamples(0), channelWeight(0.0f), samplerate(eqspec.samplerate()), eqspec_(eqspec)
{
    gains = 0.707f;
    rmsscale = 0.0f;
    rmsscalesamples = 0;
    totalSampleMean.setZero();
    
    // rmsscale for 1/1 : 4.74179792
    // rmsscale for 1/3 : 10.085021
    // rmsscale for 1/6 : 16.9321728
    // rmsscale for 1/12 : 29.1397324
    // rmsscale for 1/24 : 51.1304207
    // rmsscale for 1/48 : 91.079689
    // rmsscale for 1/96 : 164.054718
    // rmsscale for 1/192: 292.464752
}

template <size_t N>
WideIIRFilterEigen<N>::~WideIIRFilterEigen() noexcept
{
}

//==============================================================================
template <size_t N>
void WideIIRFilterEigen<N>::makeInactive() noexcept
{
    const SpinLock::ScopedLockType sl (processLock);
    active = false;
}

template <size_t N>
void WideIIRFilterEigen<N>::setCoefficients (size_t index, const IIRCoefficients& newCoefficients) noexcept
{
    const SpinLock::ScopedLockType sl (processLock);
    coefficients(index, 0) = newCoefficients.coefficients[0];
    coefficients(index, 1) = newCoefficients.coefficients[1];
    coefficients(index, 2) = newCoefficients.coefficients[2];
    coefficients(index, 3) = newCoefficients.coefficients[3];
    coefficients(index, 4) = newCoefficients.coefficients[4];
    active = true;
}

template <size_t N>
void WideIIRFilterEigen<N>::setGain(size_t index, float value) noexcept {
    gains[index] = value;
}

template <size_t N>
void WideIIRFilterEigen<N>::rms(std::vector<float>& values, std::vector<float>& gradient) {
	const SpinLock::ScopedLockType sl(processLock);
	//return rmsarray.data();
	if (values.size() == N && gradient.size() == N) {
		std::memcpy(&values[0], rmsarray.data(), N * sizeof(float));
		std::memcpy(&gradient[0], rmsgradient.data(), N * sizeof(float));
	}
}
template <size_t N>
void WideIIRFilterEigen<N>::setRms(float ms) {
	const SpinLock::ScopedLockType sl(processLock);
    rmsintegrationtime = ms;
	uint32_t numblocks = rmsintegrationtime / rmstime;
	rmsmean.size(numblocks);
	rmsgradientmean.size(numblocks);
}

//==============================================================================
template <size_t N>
void WideIIRFilterEigen<N>::reset() noexcept
{
    const SpinLock::ScopedLockType sl (processLock);
    for(auto& channel: channelState) {
        channel.v.setZero();
    }
}
//
//#if JUCE_INTEL
//#define JUCE_SNAP_TO_ZERO(n)    if (! (n < -1.0e-8 || n > 1.0e-8)) n = 0;
//#else
//#define JUCE_SNAP_TO_ZERO(n)
//#endif

template <size_t N>
void WideIIRFilterEigen<N>::resize(int numSamples, int numChannels) {
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

inline float snapToZero(float val) {
    return (val < -1.0e-8f || val > 1.0e-8f) ? val : 0.0f;
}

inline float snapToZeroSlow(float val) {
    return std::fpclassify(val) == FP_NORMAL ? val : 0.0f;
}

//==============================================================================
/**
 Helper class providing an RAII-based mechanism for temporarily disabling
 denormals on your CPU.
 */
class ForcedScopedNoDenormals
{
public:
    inline ForcedScopedNoDenormals() noexcept
    {
//#if JUCE_USE_SSE_INTRINSICS
        mxcsr = _mm_getcsr();
        _mm_setcsr (mxcsr | 0x8040); // add the DAZ and FZ bits
//#endif
    }
    
    
    inline ~ForcedScopedNoDenormals() noexcept
    {
//#if JUCE_USE_SSE_INTRINSICS
        _mm_setcsr (mxcsr);
//#endif
    }
    
private:
//#if JUCE_USE_SSE_INTRINSICS
    unsigned int mxcsr;
//#endif
};

template <size_t N>
void WideIIRFilterEigen<N>::processSamples(float* samples[], const int numSamples, const int numChannels) noexcept
{
    const SpinLock::ScopedLockType sl (processLock);
    ForcedScopedNoDenormals s;

    resize(numSamples, numChannels);

    if (active)
    {
        //Eigen::Map<Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Aligned16> inputsamples(samples, numChannels, numSamples);
		//Eigen::Map<Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Unaligned> inputsamples(samples, numChannels, numSamples);
		using arraymap = Eigen::Map<Eigen::Array<float, Eigen::Dynamic, 1>, Eigen::Unaligned>;
		std::vector<arraymap> inputsamples;
		for (int i = 0; i < numChannels; ++i)
		{
			inputsamples.emplace_back(samples[i], numSamples);
		}
        // TODO: not optimal to go over channels like this, but necessary for rms?
        //totalSampleMean.setZero();
        
        //float referenceTotalSampleMean = 0.0f;
        
        for (int i = 0; i < numSamples; ++i)
        {
            sampleMean.setZero();
            
            //float referenceSampleMean = 0.0f;
            
            for (int channel = 0; channel < numChannels; ++channel) {
                auto& channelState = this->channelState[channel];

                float& in = inputsamples[channel](i);
                out = coefficients.col(0) * in + channelState.v.col(0);

                channelState.v.col(0) = coefficients.col(1) * in - coefficients.col(3) * out + channelState.v.col(1);
                channelState.v.col(1) = coefficients.col(2) * in - coefficients.col(4) * out;
				assert(!isnan(channelState.v(0, 0)));

                // snap to zero
//#if JUCE_INTEL
                //channelState.v = channelState.v.unaryExpr(std::ref(snapToZero));
//#endif // JUCE_INTEL
                // multiply all bands outvalues with gains
                // DO NOT CHANGE AUDIO DATA
                //in = out.matrix().transpose() * gains.matrix();
                sampleMean += out * channelWeight;
                
                //referenceSampleMean += in * channelWeight;
            }
            totalSampleMean += sampleMean * sampleMean;
            numrmssamples++;
            
            //referenceTotalSampleMean += referenceSampleMean * referenceSampleMean;
            
        }
        //float referenceRms = std::sqrt(referenceTotalSampleMean * 1.0f / numSamples);
        
        if (float(numrmssamples) / samplerate >= rmstime / 1000.0f) {
            rmsarrayold = rmsarray;
            rmsarray = (totalSampleMean * 1.0f / numrmssamples).sqrt();
            rmsgradient = rmsarray - rmsarrayold;
            numrmssamples = 0;
            totalSampleMean.setZero();
			rmsmean.integrate(rmsarray, rmsarray);
			rmsgradientmean.integrate(rmsgradient, rmsgradient);
        }
        
        //float meanRms = rmsarray.mean();
        //float rmsScale = referenceRms / meanRms;
        
        //rmsscalesamples += 1;
        //rmsscale = rmsscale * float(rmsscalesamples - 1) / rmsscalesamples + rmsScale / float(rmsscalesamples);
        //int apa = 1.0;

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
const EQSpec& WideIIRFilterEigen<N>::eqspec() {
	return eqspec_;
}


#undef JUCE_SNAP_TO_ZERO


/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "EQSpec/EQSpec.h"
//#include "EQReferenceProcessor/EQReferenceProcessor.h"
//#include "EQOptimizedProcessor/EQOptimizedProcessor.h"

#include <memory>

#define THE_SEEKER_DATA_FORMAT_VERSION 0

class EQOptimizedProcessor;

//==============================================================================
/**
*/
class TheSeekerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    TheSeekerAudioProcessor();
    ~TheSeekerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setRms(float ms);

    float rms() const;
    int numbands() const;

	void setEqType(EQSpec::EQType type);

    int level() const;
    void level(int value);

	//bool showsettings() const;
	//void showsettings(bool value);

	//bool showlogo() const;
	//void showlogo(bool value);

    std::shared_ptr<EQOptimizedProcessor> eqprocessor;

	AudioProcessorValueTreeState& data();

private:
    //OwnedArray<IIRFilter> filters;
	std::vector<float*> channels;
    //std::unique_ptr<EQReferenceProcessor> eqprocessor;

	// Parameters
	//PluginData plugindata;
	AudioProcessorValueTreeState parameters;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TheSeekerAudioProcessor)
};

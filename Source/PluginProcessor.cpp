/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "EQOptimizedProcessor/EQOptimizedProcessor.h"

//==============================================================================
TheSeekerAudioProcessor::TheSeekerAudioProcessor() :
	parameters(*this, nullptr)
#ifndef JucePlugin_PreferredChannelConfigurations
    , AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	// Parameters
	parameters.createAndAddParameter("time", // parameter ID
		"Time", // parameter name
		{},
		NormalisableRange<float>(10.0f, 500.0f),
		50.0f,
		nullptr,
		nullptr,
		false,
		false);
	parameters.createAndAddParameter("res", // parameter ID
		"Res", // parameter name
		String(),
		NormalisableRange<float>(0.0f, 7.0f, 1.0f),
		1.0f,
		nullptr,
		nullptr,
		false,
		false,
		true);
	parameters.createAndAddParameter("level", // parameter ID
		"Level", // parameter name
		{},
		NormalisableRange<float>(-30.0f, 0.0f),
		0.0f,
		nullptr,
		nullptr,
		false,
		false);
	parameters.createAndAddParameter("showsettings", // parameter ID
		"Show Settings", // parameter name
		{},
		NormalisableRange<float>(0.0f, 1.0f, 1.0f),
		0.0f,
		nullptr,
		nullptr,
		false,
		false);
	parameters.createAndAddParameter("showlogo", // parameter ID
		"Show Logo", // parameter name
		{},
		NormalisableRange<float>(0.0f, 1.0f, 1.0f),
		0.0f,
		nullptr,
		nullptr,
		false,
		false);

	parameters.state = ValueTree(Identifier("TheSeekerSettings"));

	float time = *parameters.getRawParameterValue("time");
	float res = *parameters.getRawParameterValue("res");
	float level = *parameters.getRawParameterValue("level");

//    eqprocessor = std::make_unique<EQReferenceProcessor>(EQSpec(EQSpec::EQType::EQ_512_BAND_CUSTOM), getTotalNumInputChannels(), 64);
    eqprocessor = std::make_shared<EQOptimizedProcessor>(*this, EQSpec(static_cast<EQSpec::EQType>(int(*parameters.getRawParameterValue("res")) - 1)), getTotalNumInputChannels());
}

TheSeekerAudioProcessor::~TheSeekerAudioProcessor()
{
}

//==============================================================================
const String TheSeekerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TheSeekerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TheSeekerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double TheSeekerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TheSeekerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TheSeekerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TheSeekerAudioProcessor::setCurrentProgram (int index)
{
}

const String TheSeekerAudioProcessor::getProgramName (int index)
{
    return {};
}

void TheSeekerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TheSeekerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    //IIRCoefficients ic = IIRCoefficients::makeBandPass(sampleRate, 440.0);
    //filters.clear();
    //for(int i = 0; i < getTotalNumInputChannels(); ++i) {
    //    auto filter = new IIRFilter();
    //    filter->setCoefficients(ic);
    //    filters.add(filter);
    //}

    eqprocessor->prepareToPlay(sampleRate, samplesPerBlock);
}

void TheSeekerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TheSeekerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TheSeekerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    //float* channels[totalNumInputChannels];

	//float* channels[totalNumInputChannels];
	if (channels.size() != totalNumInputChannels) {
		channels.resize(totalNumInputChannels);
	}
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        channels[channel] = buffer.getWritePointer (channel);
        //filters[channel]->processSamples(channelData, buffer.getNumSamples());
    }
    eqprocessor->processSamples(channels.data(), buffer.getNumSamples());
}

//==============================================================================
bool TheSeekerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TheSeekerAudioProcessor::createEditor()
{
    return new TheSeekerAudioProcessorEditor (*this);
}

//==============================================================================
void TheSeekerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	auto state = parameters.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void TheSeekerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(parameters.state.getType()))
			parameters.replaceState(ValueTree::fromXml(*xmlState));
	eqprocessor->setRms(*parameters.getRawParameterValue("time"));
	eqprocessor->setEqType(static_cast<EQSpec::EQType>(std::max(0, int(*parameters.getRawParameterValue("res")) - 1)));
	eqprocessor->level(*parameters.getRawParameterValue("level"));
}

void TheSeekerAudioProcessor::setRms(float ms) 
{
	//*plugindata.time = ms;
	//parameters.getParameterAsValue("time").setValue(ms);
    eqprocessor->setRms(ms);
}

float TheSeekerAudioProcessor::rms() const 
{
	return *parameters.getRawParameterValue("time");
}

int TheSeekerAudioProcessor::numbands() const
{
    return eqprocessor->numbands();
}

void TheSeekerAudioProcessor::setEqType(EQSpec::EQType type) 
{
	//*plugindata.res = static_cast<int>(type) + 1;
	//parameters.getParameterAsValue("res").setValue(static_cast<int>(type) + 1);
	eqprocessor->setEqType(type);
}

int TheSeekerAudioProcessor::level() const 
{
    return *parameters.getRawParameterValue("level");
}

void TheSeekerAudioProcessor::level(int value) 
{
    eqprocessor->level(value);
}

//bool TheSeekerAudioProcessor::showsettings() const
//{
//
//}
//
//void TheSeekerAudioProcessor::showsettings(bool value)
//{
//
//}
//
//bool TheSeekerAudioProcessor::showlogo() const 
//{
//
//}
//
//void TheSeekerAudioProcessor::showlogo(bool value) 
//{
//
//}

AudioProcessorValueTreeState& TheSeekerAudioProcessor::data() 
{
	return this->parameters;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TheSeekerAudioProcessor();
}

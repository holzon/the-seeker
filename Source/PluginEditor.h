/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "UILayer/UILayer.h"
#include "SpectrumGraphComponent/SpectrumGraphComponent.h"
#include "EQControlComponent/EQControlComponent.h"
#include "LookAndFeel/TheSeekerLookAndFeel.h"

//==============================================================================
/**
*/
class TheSeekerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    TheSeekerAudioProcessorEditor (TheSeekerAudioProcessor&);
    ~TheSeekerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    SpinLock processorLock;
    TheSeekerLookAndFeel lookAndFeel;
    OpenGLContext openGLContext;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TheSeekerAudioProcessor& processor;
    
    SpectrumGraphComponent spectrumgraph;
    EQControlComponent eqcontrol;
    UILayer uilayer;
    int margin;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TheSeekerAudioProcessorEditor)
};

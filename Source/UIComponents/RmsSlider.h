/*
  ==============================================================================

    RmsSlider.h
    Created: 3 Oct 2017 6:40:16pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "SliderLabel.h"

//==============================================================================
/*
*/

class RmsSlider    : public Component, private Slider::Listener
{
public:
    RmsSlider(TheSeekerAudioProcessor& processor);
    ~RmsSlider();

    void paint (Graphics&) override;
    void resized() override;

    Slider& getSlider();

    void sliderValueChanged(Slider* slider) override;
    void sliderDragEnded(Slider* slider) override;

private:
    TheSeekerAudioProcessor& processor;
    Slider slider;
    SliderLabel sliderlabelwhite;
	AudioProcessorValueTreeState::SliderAttachment attachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RmsSlider)
};

/*
  ==============================================================================

    OctaveSelect.h
    Created: 3 Oct 2017 11:27:19pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

//==============================================================================
/*
*/
class OctaveSelect    : public Component, private ComboBox::Listener
{
public:
    OctaveSelect(TheSeekerAudioProcessor& processor);
    ~OctaveSelect();

    void paint (Graphics&) override;
    void resized() override;
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;

private:
    ComboBox octaves;
	TheSeekerAudioProcessor& processor;
	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> attachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OctaveSelect)
};

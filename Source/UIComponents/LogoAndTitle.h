/*
  ==============================================================================

    LabsLogo.h
    Created: 4 Oct 2017 11:14:25am
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "LabsLogo.h"
#include "../PluginProcessor.h"

//==============================================================================
/*
*/
class LogoAndTitle    : public Button
{
public:
    LogoAndTitle(TheSeekerAudioProcessor& processor);
    ~LogoAndTitle();

	void paintButton(Graphics& g,
		bool isMouseOverButton,
		bool isButtonDown) override;
    void resized() override;
	void clicked() override;
private:
	void updateVisibleState();
    Label title;
    LabsLogo logo;
	std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> attachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LogoAndTitle)
};

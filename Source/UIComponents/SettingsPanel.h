/*
  ==============================================================================

    SettingsPanel.h
    Created: 4 Oct 2017 11:14:25am
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../UIComponents/OctaveSelect.h"
#include "../UIComponents/RmsSlider.h"
#include "../UIComponents/LevelSlider.h"

//==============================================================================
/*
*/
class SettingsPanel    : public Button
{
public:
    SettingsPanel(TheSeekerAudioProcessor& processor);
    ~SettingsPanel();

    void paint (Graphics&) override;
	void paintButton(Graphics& g,
		bool isMouseOverButton,
		bool isButtonDown) override;
    void resized() override;
	void clicked() override;
private:
    void updateVisibleState();
    bool visible = true;
	TheSeekerAudioProcessor& processor;
    OctaveSelect octaveselect;
    RmsSlider rmsslider;
    LevelSlider levelslider;
    DrawablePath hide_triangle;
	DrawablePath show_triangle;
	std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> attachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsPanel)
};

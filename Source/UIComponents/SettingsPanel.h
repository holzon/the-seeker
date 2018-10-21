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

//==============================================================================
/*
*/
class SettingsPanel    : public Component
{
public:
    SettingsPanel(TheSeekerAudioProcessor& processor);
    ~SettingsPanel();

    void paint (Graphics&) override;
    void resized() override;
    void mouseUp(const MouseEvent&) override;
private:
    void updateVisibleState();
    bool visible = true;
    OctaveSelect octaveselect;
    RmsSlider rmsslider;
    DrawablePath hide_triangle;
	DrawablePath show_triangle;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsPanel)
};

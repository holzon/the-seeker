/*
  ==============================================================================

    UILayer.h
    Created: 29 Sep 2017 8:49:45pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "../UIComponents/LogoAndTitle.h"
#include "../UIComponents/ModeSelect.h"
#include "../UIComponents/OctaveSelect.h"
#include "../UIComponents/RmsSlider.h"

//==============================================================================
/*
*/
class UILayer    : public Component
{
public:
    UILayer(TheSeekerAudioProcessor& processor);
    ~UILayer();

    void paint (Graphics&) override;
    void resized() override;

private:
    LogoAndTitle logoandtitle;
    ModeSelect modeselect;
    OctaveSelect octaveselect;
    RmsSlider rmsslider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UILayer)
};

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
#include "../UIComponents/SettingsPanel.h"

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
    SettingsPanel settings;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UILayer)
};

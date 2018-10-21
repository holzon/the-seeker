/*
  ==============================================================================

    SliderLabel.h
    Created: 3 Oct 2017 6:40:16pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

//==============================================================================
/*
*/

class SliderLabel : public Component
{
public:
    SliderLabel();
    ~SliderLabel();
    
    void setSlider(Slider& slider);
    
    void paint (Graphics&) override;
    
private:
    Slider* slider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderLabel)
};
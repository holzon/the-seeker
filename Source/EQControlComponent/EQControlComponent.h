/*
  ==============================================================================

    EQControlComponent.h
    Created: 26 Apr 2018 8:53:43pm
    Author:  jensg

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class EQControlComponent    : public Component
{
public:
    EQControlComponent();
    ~EQControlComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQControlComponent)
};

/*
  ==============================================================================

    ModeSelect.h
    Created: 2 Oct 2017 9:01:19pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MultiButton.h"

//==============================================================================
/*
*/
class ModeSelect : public Component
{
public:
    ModeSelect();
    ~ModeSelect();

    void paint (Graphics&) override;
    void resized() override;

private:
    MultiButton mb;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModeSelect)
};

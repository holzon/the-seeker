/*
  ==============================================================================

    MultiButton.h
    Created: 2 Oct 2017 9:39:09pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MultiButton    : public Component
{
public:
    MultiButton(int uniqueid);
    ~MultiButton();
    
    MultiButton& add(const String& label);

    void paint (Graphics&) override;
    void resized() override;

private:
    int uniqueid;
    int selected;
    Array<String> labels;
    OwnedArray<Button> buttons;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiButton)
};

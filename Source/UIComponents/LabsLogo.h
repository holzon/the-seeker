/*
  ==============================================================================

    LabsLogo.h
    Created: 4 Oct 2017 11:14:25am
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class LabsLogo    : public Component
{
public:
    LabsLogo();
    ~LabsLogo();

    void paint (Graphics&) override;
    void resized() override;

private:
    void createLogo();
    std::shared_ptr<Drawable> logo;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabsLogo)
};

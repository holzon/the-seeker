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

//==============================================================================
/*
*/
class LogoAndTitle    : public Component, public MouseListener
{
public:
    LogoAndTitle();
    ~LogoAndTitle();

    void paint (Graphics&) override;
    void resized() override;
    void mouseUp(const MouseEvent&) override;
private:
    Label title;
    LabsLogo logo;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LogoAndTitle)
};

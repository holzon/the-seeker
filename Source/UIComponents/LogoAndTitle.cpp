/*
  ==============================================================================

    LabsLogo.cpp
    Created: 4 Oct 2017 11:14:25am
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "LogoAndTitle.h"

//==============================================================================
LogoAndTitle::LogoAndTitle()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(title);
    addAndMakeVisible(logo);
    title.setText ("THE SEEKER", dontSendNotification);
    title.setJustificationType(Justification::topRight);
    logo.setInterceptsMouseClicks(false, false);
    title.setInterceptsMouseClicks(false, false);
}

LogoAndTitle::~LogoAndTitle()
{
}

void LogoAndTitle::paint (Graphics& g)
{

}

void LogoAndTitle::mouseUp(const MouseEvent& event)
{
    logo.setVisible(!logo.isVisible());
    title.setVisible(!title.isVisible());
}

void LogoAndTitle::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto area (getLocalBounds());
    //auto top (area.removeFromTop (48));
    //auto bottom (area.removeFromBottom (48));
    //auto topleft (top.removeFromLeft(148));
    //auto toplefttop = topleft.removeFromTop(28);
    //modeselect.setBounds(toplefttop);
    
    //auto topright1 (top.removeFromRight(192));
    logo.setBounds(area);
    
    //auto rightpanel (area.removeFromRight(108));
    area.removeFromTop(48);
    auto titlepanel (area.removeFromTop(28));
    titlepanel.translate(0, -16);
    title.setBounds(titlepanel);
}

/*
  ==============================================================================

    UILayer.cpp
    Created: 29 Sep 2017 8:49:45pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "UILayer.h"

//==============================================================================
UILayer::UILayer(TheSeekerAudioProcessor& processor) :
settings(processor)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    //setOpaque(false);

    //addAndMakeVisible(modeselect);
    addAndMakeVisible(logoandtitle);
    addAndMakeVisible(settings);
}

UILayer::~UILayer()
{
}

void UILayer::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
}

void UILayer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
	auto completearea(getLocalBounds());
    auto area (getLocalBounds().reduced (12));
    auto logoarea (area);
    logoarea = (logoarea.removeFromRight(192));
    logoarea = (logoarea.removeFromTop(76));
    logoandtitle.setBounds(logoarea);

    auto top (area.removeFromTop (48));
    auto bottom (area.removeFromBottom (48));
    auto topleft (top.removeFromLeft(148));
    auto toplefttop = topleft.removeFromTop(28);
    //modeselect.setBounds(toplefttop);
    
    auto topright1 (top.removeFromRight(192));
    top.removeFromRight(8);
    auto topright2 (top.removeFromRight(96));
    top.removeFromRight(4);
    auto topright3 (top.removeFromRight(96));
    
    auto rightpanel (completearea.removeFromRight(132));
    rightpanel = rightpanel.withSizeKeepingCentre(rightpanel.getWidth(), rightpanel.getHeight() - 200);
    settings.setBounds(rightpanel);

//    area.removeFromTop(8);
//    auto belowtop = area.removeFromTop(48);
//    auto belowtopright (belowtop.removeFromRight(192));
//    area.removeFromTop(8);
//    auto belowtop2 = area.removeFromTop(28);
//    auto belowtop2right (belowtop2.removeFromRight(96));
//    title.setBounds(belowtop2right);
    //hello.setBounds (top);
}

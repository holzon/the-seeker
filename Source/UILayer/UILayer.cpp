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
octaveselect(processor),
rmsslider(processor)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    //setOpaque(false);
    addAndMakeVisible(title);
    addAndMakeVisible(logo);
    //addAndMakeVisible(modeselect);
    addAndMakeVisible(octaveselect);
    addAndMakeVisible(rmsslider);
    title.setText ("THE SEEKER", dontSendNotification);
    title.setJustificationType(Justification::topRight);
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
    auto area (getLocalBounds().reduced (12));

    auto top (area.removeFromTop (48));
    auto bottom (area.removeFromBottom (48));
    auto topleft (top.removeFromLeft(148));
    auto toplefttop = topleft.removeFromTop(28);
    //modeselect.setBounds(toplefttop);
    
    auto topright1 (top.removeFromRight(192));
    logo.setBounds(topright1);
    top.removeFromRight(8);
    auto topright2 (top.removeFromRight(96));
    top.removeFromRight(4);
    auto topright3 (top.removeFromRight(96));
    
    auto rightpanel (area.removeFromRight(108));
    auto rightpaneltemp = rightpanel;
    
    int numitems = 2;
    int itemheight = numitems * 28 + 8;
    int itemoffset = (rightpanel.getHeight() - itemheight) * 0.5f;
    
    auto rightpanel1 (rightpaneltemp.removeFromTop(28));
    rightpanel1.translate(0, -16);
    title.setBounds(rightpanel1);
    // offset items
    rightpanel.removeFromTop(itemoffset);
    auto rightpanel2 (rightpanel.removeFromTop(28));
    rightpanel.removeFromTop(8);
    auto rightpanel3 (rightpanel.removeFromTop(28));

    rmsslider.setBounds(rightpanel2);
    octaveselect.setBounds(rightpanel3);
    
//    area.removeFromTop(8);
//    auto belowtop = area.removeFromTop(48);
//    auto belowtopright (belowtop.removeFromRight(192));
//    area.removeFromTop(8);
//    auto belowtop2 = area.removeFromTop(28);
//    auto belowtop2right (belowtop2.removeFromRight(96));
//    title.setBounds(belowtop2right);
    //hello.setBounds (top);
}

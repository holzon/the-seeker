/*
  ==============================================================================

    EQControlComponent.cpp
    Created: 26 Apr 2018 8:53:43pm
    Author:  jensg

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "EQControlComponent.h"

//==============================================================================
EQControlComponent::EQControlComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

EQControlComponent::~EQControlComponent()
{
}

void EQControlComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("EQControlComponent", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void EQControlComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

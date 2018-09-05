/*
  ==============================================================================

    ModeSelect.cpp
    Created: 2 Oct 2017 9:01:19pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "ModeSelect.h"

ModeSelect::ModeSelect() :
mb(6661) {
    mb.add("SCOPE").add("EQ").add(" SIDE");
    addAndMakeVisible(mb);
}

//==============================================================================

ModeSelect::~ModeSelect()
{

}

void ModeSelect::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

}

void ModeSelect::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    mb.setBounds(getLocalBounds());
}

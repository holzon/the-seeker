/*
  ==============================================================================

    OctaveSelect.cpp
    Created: 3 Oct 2017 11:27:19pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "OctaveSelect.h"

//==============================================================================
OctaveSelect::OctaveSelect(TheSeekerAudioProcessor& processor) :
processor(processor)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
//    octaves.setColour (ComboBox::textColourId, Colours::white);
//    octaves.setColour (ComboBox::arrowColourId, Colours::white);
//    octaves.setColour (ComboBox::backgroundColourId, Colours::transparentBlack);
//    octaves.setColour (ComboBox::outlineColourId, Colours::transparentBlack);
    octaves.addItem("1/1 octave", 1);
    octaves.addItem("1/3 octave", 2);
    octaves.addItem("1/6 octave", 3);
    octaves.addItem("1/12 octave", 4);
    octaves.addItem("1/24 octave", 5);
    octaves.addItem("1/48 octave", 6);
    octaves.addItem("1/96 octave", 7);
    octaves.addItem("1/192 octave", 8);
    //octaves.setSelectedId(6, dontSendNotification);
    octaves.addListener(this);
	attachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.data(), "res", octaves));
    addAndMakeVisible(octaves);
}

OctaveSelect::~OctaveSelect()
{
}

void OctaveSelect::paint (Graphics& g)
{
    // Draw border
    g.setColour (Colours::white);
    auto bounds = getLocalBounds().reduced(2);
    Path path;
    constexpr float cornerSize = 6.0f;
    path.addRoundedRectangle (bounds.getX(), bounds.getY(),
                              bounds.getWidth(), bounds.getHeight(),
                              cornerSize);
    g.strokePath (path, PathStrokeType (2.0f));
}

void OctaveSelect::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto bounds = getLocalBounds().reduced(4);
    octaves.setBounds(bounds);
}

void OctaveSelect::comboBoxChanged (ComboBox* comboBoxThatHasChanged) {
    processor.setEqType(static_cast<EQSpec::EQType>(comboBoxThatHasChanged->getSelectedId() - 1));
}

/*
  ==============================================================================

    MultiButton.cpp
    Created: 2 Oct 2017 9:39:09pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MultiButton.h"

//==============================================================================
MultiButton::MultiButton(int uniqueid) :
uniqueid(uniqueid),
selected(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

MultiButton::~MultiButton()
{
}

MultiButton& MultiButton::add(const String& label) {
    labels.add(label);
    std::unique_ptr<TextButton> button = std::make_unique<TextButton>(label);
    button->setClickingTogglesState(true);
    button->setRadioGroupId(uniqueid);
//    button->setColour (TextButton::textColourOffId, Colours::white);
//    button->setColour (TextButton::textColourOnId, Colours::black);
//    button->setColour (TextButton::buttonColourId, Colours::transparentBlack);
//    button->setColour (TextButton::buttonOnColourId, Colours::white);

    if(buttons.size() == 0) {
        button->setToggleState(true, dontSendNotification);
    } else {
        // adjust edge on prev button
        const int index = buttons.size() - 1;
        buttons[index]->setConnectedEdges((index > 0 ? Button::ConnectedOnLeft : 0) | Button::ConnectedOnRight);
        button->setConnectedEdges(Button::ConnectedOnLeft);
    }
    addAndMakeVisible(button.get());
    buttons.add(button.release());
    resized();
    return *this;
}

void MultiButton::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    // Draw border
    g.setColour (Colours::white);
    //g.drawRoundedRectangle(getLocalBounds().toFloat(), 3.0f, 2.0f);   // draw an outline around the component
    auto bounds = getLocalBounds().reduced(2);
    Path path;
    constexpr float cornerSize = 6.0f;
    path.addRoundedRectangle (bounds.getX(), bounds.getY(),
                                  bounds.getWidth(), bounds.getHeight(),
                                  cornerSize);
    g.strokePath (path, PathStrokeType (2.0f));
    
//    Rectangle<int> multibuttonsarea = getLocalBounds().reduced(4);
//    Rectangle<int> buttonarea = multibuttonsarea;
//    const float buttonwidth = 1.0 / labels.size() * multibuttonsarea.getWidth();
//    for (int i = 0; i < labels.size(); ++i) {
//        buttonarea.setLeft(multibuttonsarea.getX() + i * buttonwidth);
//        buttonarea.setWidth(buttonwidth);
//        g.setColour(Colours::white);
//        g.setFont(12.0f);
//        g.drawText(labels[i], buttonarea,
//                    Justification::centred, true);   // draw some placeholder text
//    }
}

void MultiButton::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> multibuttonsarea = getLocalBounds().reduced(4);
    Rectangle<int> buttonarea = multibuttonsarea;
    const float buttonwidth = 1.0 / labels.size() * multibuttonsarea.getWidth();
    for (int i = 0; i < buttons.size(); ++i) {
        buttonarea.setLeft(multibuttonsarea.getX() + i * buttonwidth);
        buttonarea.setWidth(buttonwidth);
        buttons[i]->setBounds(buttonarea);
    }
}

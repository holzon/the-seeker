/*
  ==============================================================================

    RmsSlider.cpp
    Created: 3 Oct 2017 6:40:16pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "RmsSlider.h"

SliderLabel::SliderLabel() {
    setInterceptsMouseClicks(false, false);
}
SliderLabel::~SliderLabel() { }

void SliderLabel::setSlider(Slider& slider) {
    this->slider = &slider;
}

void SliderLabel::paint (Graphics& g) {
    if (slider) {
        auto bounds = getLocalBounds();
        auto value = slider->getValue();
        auto label = slider->getTextFromValue(value);
        g.setFont(12.0f);
        g.saveState();
        g.setColour(Colours::white);
        g.excludeClipRegion(bounds.removeFromLeft(slider->getPositionOfValue(value)));
        g.drawText(label, getLocalBounds(), Justification::centred);
        g.restoreState();
        g.setColour(Colours::black);
        g.excludeClipRegion(bounds);
        g.drawText(label, getLocalBounds(), Justification::centred);
    }
}


//==============================================================================
RmsSlider::RmsSlider(TheSeekerAudioProcessor& processor) :
processor(processor),
attachment(processor.data(), "time", slider) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    //slider.setRange (10.0, 500.0, 1.0);
    //slider.setValue(10.0);
    slider.setSliderStyle (Slider::LinearBar);
    slider.setTextBoxStyle(Slider::NoTextBox, true, 1, 1);
    slider.setTextValueSuffix (" ms");
	slider.setNumDecimalPlacesToDisplay(0);
//    slider.setColour (Slider::trackColourId, Colours::white);
//    slider.setColour (Slider::textBoxOutlineColourId, Colours::transparentBlack);
//    slider.setColour (Slider::textBoxTextColourId, Colours::white);
    slider.addListener(this);
    addAndMakeVisible(slider);
    sliderlabelwhite.setSlider(slider);
    addAndMakeVisible(sliderlabelwhite);
}

RmsSlider::~RmsSlider()
{
}

void RmsSlider::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.setColour (Colours::white);
    //g.drawRoundedRectangle(getLocalBounds().toFloat(), 3.0f, 2.0f);   // draw an outline around the component
    auto bounds = getLocalBounds().reduced(2);
    Path path;
    constexpr float cornerSize = 6.0f;
    path.addRoundedRectangle (bounds.getX(), bounds.getY(),
                              bounds.getWidth(), bounds.getHeight(),
                              cornerSize);
    g.strokePath (path, PathStrokeType (2.0f));
}

void RmsSlider::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> bounds = getLocalBounds().reduced(3);
    slider.setBounds(bounds);
    sliderlabelwhite.setBounds(bounds);
}

Slider& RmsSlider::getSlider() {
    return slider;
}

void RmsSlider::sliderValueChanged(Slider* slider) {
    processor.setRms(slider->getValue());
}

void RmsSlider::sliderDragEnded(Slider* slider) {
    processor.setRms(slider->getValue());
}


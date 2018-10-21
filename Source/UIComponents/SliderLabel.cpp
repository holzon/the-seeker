/*
  ==============================================================================

    SliderLabel.cpp
    Created: 3 Oct 2017 6:40:16pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SliderLabel.h"

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

/*
  ==============================================================================

    SpectrumGraphComponent.h
    Created: 5 Sep 2017 7:32:51pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include <vector>

#include "../EQProcessor/EQProcessor.h"

class SpectrumGraphComponent  : public Component, public Timer
{
public:
    SpectrumGraphComponent(std::shared_ptr<EQProcessor> eq, int bands = 33, int gutter = 4);
    ~SpectrumGraphComponent();
    
    void timerCallback() override;

    void paint (Graphics&) override;
    void resized() override;

	void mouseExit(const MouseEvent &event) override;
	void mouseMove(const MouseEvent &event) override;

    void setnumbands(int bands);

private:
	SpinLock bufferlock;
    //OwnedArray<DrawableRectangle> bands;
    ColourGradient gradient;
    int gutter;
    std::shared_ptr<EQProcessor> eq;
    EQSpec eqspec;
    std::vector<float> rms;
    std::vector<float> rmsgradient;
    float rmsscale;
	bool displaydata = false;
	int mouse_x = 0;
	int mouse_y = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumGraphComponent)
};

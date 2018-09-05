/*
  ==============================================================================

    SpectrumGraphComponentGL.h
    Created: 29 Sep 2017 5:45:48pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include <vector>

#include "../EQProcessor/EQProcessor.h"

class SpectrumGraphComponentGL  : public Component, private OpenGLRenderer
{
public:
    SpectrumGraphComponentGL(EQProcessor& eq, int bands = 33, int gutter = 4);
    ~SpectrumGraphComponentGL();
    
    void paint (Graphics&) override;
    void resized() override;
    void setnumbands(int bands);
    
    void updateBars();
    
private:
    OpenGLContext openGLContext;
    OwnedArray<DrawableRectangle> bands;
    Array<Rectangle<float>> rectangles;
    ColourGradient gradient;
    int gutter;
    EQProcessor& eq;
    std::vector<float> rms;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumGraphComponentGL)
};

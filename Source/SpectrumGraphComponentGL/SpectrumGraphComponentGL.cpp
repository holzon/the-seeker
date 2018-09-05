/*
  ==============================================================================

    SpectrumGraphComponentGL.cpp
    Created: 29 Sep 2017 5:45:48pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "SpectrumGraphComponentGL.h"

#include <cmath>

SpectrumGraphComponentGL::SpectrumGraphComponentGL(EQProcessor& eq, int bands, int gutter) : gutter(gutter), eq(eq) {
    setnumbands(bands);
    gradient.addColour(0.0f, Colour(0.32f, 0.6f, 0.5f, 1.0f));
    gradient.addColour(0.25f, Colour(0.24f, 0.7f, 0.8f, 1.0f));
    gradient.addColour(0.5f, Colour(0.16f, 0.8f, 0.9f, 1.0f));
    gradient.addColour(0.75f, Colour(0.08f, 0.9f, 0.95f, 1.0f));
    gradient.addColour(1.0f, Colour(0.0f, 1.0f, 1.0f, 1.0f));

    setOpaque (true);
    
    openGLContext.setRenderer (this);
    openGLContext.attachTo (*this);
    openGLContext.setContinuousRepainting (true);

    openGLContext.setMultisamplingEnabled(true);
    OpenGLPixelFormat format;
    format.multisamplingLevel = 4;
    openGLContext.setPixelFormat(format);
}

SpectrumGraphComponentGL::~SpectrumGraphComponentGL() {
}

void SpectrumGraphComponentGL::paint (Graphics& g) {
    //return;
    //updateBars();
    
    g.setImageResamplingQuality(Graphics::highResamplingQuality);
    
    float gutter = 0.0f;
    if (getWidth() / float(bands.size()) >= 6.0f) {
        gutter = getWidth() * 0.2f / bands.size();
    }
    
    const auto numgutters = bands.size() - 1;
    const auto totalguttersize = numgutters * gutter;
    const auto bandpixelwidth = std::max(0.0f, float(getWidth() - totalguttersize) / bands.size());
    const auto bandpixelheight = std::max(2, getHeight());
    const auto bandpixeloffset = bandpixelwidth + gutter;
    
    gradient.point1 = Point<float>(0.0f, bandpixelheight);
    gradient.point2 = Point<float>(0.0f, 0.0f);
    for (int i = 0; i < bands.size(); ++i) {
        //const auto bandyoffset = float(bands.size() - 1 - i) * bandpixelheight / bands.size();
        //const auto bandyoffset = (1.0 - 4.0 * log(1.0 * rms[i] + 1.0)) * bandpixelheight;
        const auto bandyoffset = (1.0 - (0.015 * Decibels::gainToDecibels(rms[i]) + 1.0)) * bandpixelheight;
        const auto bandheight =(bands.size() + 1.0f) * bandpixelheight / bands.size();
        g.setColour(Colour( float(i) / bands.size(), 1.0f, 1.0f, 1.0f));
        //g.setGradientFill(gradient);
        
        
        g.drawLine(i * bandpixeloffset, bandyoffset, i * bandpixeloffset, bandheight, bandpixelwidth);
        //g.drawVerticalLine(i * bandpixeloffset + bandpixelwidth * 0.5f, bandyoffset, bandyoffset + bandheight);
        
        //g.fillRect(Rectangle<float>(i * bandpixeloffset, bandyoffset, bandpixeloffset * 1.001f, bandheight));
        
        
        
        //bands[i]->setRectangle(Rectangle<float> (i * bandpixeloffset, bandyoffset, bandpixelwidth, bandheight));
        //bands[i]->setRectangle(Rectangle<float> (i * bandpixeloffset, bandyoffset, bandpixelwidth, bandheight));
        //gradient.point1 = Point<float>(0.0f, bandpixelheight);
        //gradient.point2 = Point<float>(0.0f, 0.0f);
        //bands[i]->setFill(gradient);
        //bands[i]->setFill(ColourGradient(Colour(0.333f, 0.5f, 1.0f, 1.0f), Point<float>(0.0f, bandyoffset), Colour(0.5f, 1.0f, 0.0f, 1.0f), Point<float>(0.0f, bandyoffset + bandpixelheight * 0.05f), false));
    }
}

void SpectrumGraphComponentGL::resized() {
    //updatebars();
}

void SpectrumGraphComponentGL::updateBars() {
    auto gutter = getWidth() * 0.2f / bands.size();
    const auto numgutters = bands.size() - 1;
    const auto totalguttersize = numgutters * gutter;
    const auto bandpixelwidth = std::max(2.0f, float(getWidth() - totalguttersize) / bands.size());
    const auto bandpixelheight = std::max(2, getHeight());
    const auto bandpixeloffset = bandpixelwidth + gutter;
    for (int i = 0; i < bands.size(); ++i) {
        //const auto bandyoffset = float(bands.size() - 1 - i) * bandpixelheight / bands.size();
        const auto bandyoffset = (1.0 - 4.0 * log(1.0 * rms[i] + 1.0)) * bandpixelheight;
        const auto bandheight =(bands.size() + 1.0f) * bandpixelheight / bands.size();
        //bands[i]->setRectangle(Rectangle<float> (i * bandpixeloffset, bandyoffset, bandpixelwidth, bandheight));
        bands[i]->setRectangle(Rectangle<float> (i * bandpixeloffset, bandyoffset, bandpixelwidth, bandheight));
        gradient.point1 = Point<float>(0.0f, bandpixelheight);
        gradient.point2 = Point<float>(0.0f, 0.0f);
        bands[i]->setFill(gradient);
        //bands[i]->setFill(ColourGradient(Colour(0.333f, 0.5f, 1.0f, 1.0f), Point<float>(0.0f, bandyoffset), Colour(0.5f, 1.0f, 0.0f, 1.0f), Point<float>(0.0f, bandyoffset + bandpixelheight * 0.05f), false));
    }
}

void SpectrumGraphComponentGL::setnumbands(int numbands) {
    if (bands.size() < numbands) {
        for (int i = bands.size(); i < numbands; ++i) {
            auto rect = new DrawableRectangle();
            //rect->setCornerSize(RelativePoint(0.0, 0.0);//(2.0f, 2.0f));
            addAndMakeVisible(rect);
            bands.add(rect);
        }
        resized();
    } else if (numbands < bands.size()) {
        for (int i = numbands; i < bands.size(); ++i) {
            removeChildComponent(bands[i]);
        }
        bands.removeRange(numbands, bands.size() - numbands);
        resized();
    } // else numbands == bands.size(); all is fine
    
    rectangles.resize(numbands);
    rms.resize(numbands, 0.0f);
    resized();
}

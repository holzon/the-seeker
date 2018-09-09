/*
  ==============================================================================

    SpectrumGraphComponent.cpp
    Created: 5 Sep 2017 7:32:51pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "SpectrumGraphComponent.h"

#include <array>
#include <cmath>
#include <string>

namespace {

	//static std::array<float, 8> EQCalibration { 6.0f, 10.085021f, 16.9321728f, 29.1397324f, 51.1304207f, 91.079689f, 164.054718f, 292.464752f };
	// static std::array<float, 8> EQCalibration{ 1.3065629648763766f, 3.1136297131333874f, 6.057518690855654f, 12.028850466519177f, 24.014436726084917f, 48.00721980289169f, 96.00361008152873f, 192.00180506327794f };
	//static std::array<float, 8> EQCalibration{ 1.0f, 3.0f, 6.0f, 12.0f, 24.0f, 48.0f, 96.0f, 192.0f };
	static std::array<float, 8> EQCalibration { 1.0f, 1.7320508075688772f, 2.449489742783178f, 3.4641016151377544f, 4.898979485566356f, 6.928203230275509f, 9.797958971132712f, 13.856406460551018f };
    
}

SpectrumGraphComponent::SpectrumGraphComponent(std::shared_ptr<EQProcessor> eq, int bands, int gutter) : gutter(gutter), eq(eq), rmsscale(0.0f) {
    setnumbands(bands);
    gradient.addColour(0.0f, Colour(0.32f, 0.6f, 0.5f, 1.0f));
    gradient.addColour(0.25f, Colour(0.24f, 0.7f, 0.8f, 1.0f));
    gradient.addColour(0.5f, Colour(0.16f, 0.8f, 0.9f, 1.0f));
    gradient.addColour(0.75f, Colour(0.08f, 0.9f, 0.95f, 1.0f));
    gradient.addColour(1.0f, Colour(0.0f, 1.0f, 1.0f, 1.0f));

    setOpaque (true);
    


    //openGLContext.setContinuousRepainting (true);
    startTimer(5);
}

SpectrumGraphComponent::~SpectrumGraphComponent() {
    
}

void SpectrumGraphComponent::timerCallback() {
	{
		const SpinLock::ScopedLockType sl(bufferlock);
		eq->rms(rms, rmsgradient);
	}
    repaint();
}

void SpectrumGraphComponent::paint (Graphics& g) {
    const int numbands = eq->numbands();
    if (numbands != rms.size()) {
        setnumbands(numbands);
    }
    g.fillAll (Colour (0xff080808));
    //return;
    //updateBars();

    g.setImageResamplingQuality(Graphics::highResamplingQuality);
    
    float gradientsatoffset = 0.25f;
    float gradientsatscale = 75.0f;
    float gradientvaloffset = 0.1f;
    float gradientvalscale = 100.0f;
    
    float gutter = 0.0f;
    if (getWidth() / float(rms.size()) >= 4.0f) {
        gutter = getWidth() * 0.2f / rms.size();
    }
    
    const auto numgutters = rms.size() - 1;
    const auto totalguttersize = numgutters * gutter;
    const auto bandpixelwidth = std::max(0.0f, float(getWidth() - totalguttersize) / rms.size());
    const auto bandpixelheight = std::max(2, getHeight());
    const auto bandpixeloffset = bandpixelwidth + gutter;
    
    const float dBmax = 10.0f;
    const float dBmin = -70.0f;
    const float dbScale = (bandpixelheight - 1)/(dBmax - dBmin);
    
    gradient.point1 = Point<float>(0.0f, bandpixelheight);
    gradient.point2 = Point<float>(0.0f, 0.0f);
    if (gutter == 0.0f) {
        const float weight = 1.0f / getWidth();
        const auto numbands = rms.size();
        for (int i = 0; i < getWidth(); ++i) {
            int band = i * weight * numbands;
            //const auto bandyoffset = float(bands.size() - 1 - i) * bandpixelheight / bands.size();
            //const auto bandyoffset = (1.0 - 4.0 * log(1.0 * rms[i] + 1.0)) * bandpixelheight;
            //const auto bandyoffset = (1.0 - (0.015 * Decibels::gainToDecibels(rms[band] * rmsscale) + 0.75)) * bandpixelheight;
            const auto bandyoffset = -dbScale * (Decibels::gainToDecibels(rms[band] * rmsscale) - dBmax);
            //const auto bandheight =(rms.size() + 1.0f) * bandpixelheight / rms.size();
            const auto bandheight = bandpixelheight - bandyoffset;
            //g.setColour(Colour( 0.75f * float(band) / bands.size(), 1.0f, 1.0f, 1.0f));
            g.setColour(Colour( 0.75f * float(band) / rms.size(), gradientsatoffset + gradientsatscale * std::abs(rmsgradient[band]), gradientvaloffset + gradientvalscale * std::abs(rmsgradient[band]), 1.0f));
            //g.setGradientFill(gradient);


            //g.drawLine(band * bandpixeloffset, bandyoffset, band * bandpixeloffset, bandheight, bandpixelwidth);
            g.drawVerticalLine(i, bandyoffset, bandyoffset + bandheight);

            //g.fillRect(Rectangle<float>(i * bandpixeloffset, bandyoffset, bandpixeloffset * 1.00f, bandheight));



            //bands[i]->setRectangle(Rectangle<float> (i * bandpixeloffset, bandyoffset, bandpixelwidth, bandheight));
            //bands[i]->setRectangle(Rectangle<float> (i * bandpixeloffset, bandyoffset, bandpixelwidth, bandheight));
            //gradient.point1 = Point<float>(0.0f, bandpixelheight);
            //gradient.point2 = Point<float>(0.0f, 0.0f);
            //bands[i]->setFill(gradient);
            //bands[i]->setFill(ColourGradient(Colour(0.333f, 0.5f, 1.0f, 1.0f), Point<float>(0.0f, bandyoffset), Colour(0.5f, 1.0f, 0.0f, 1.0f), Point<float>(0.0f, bandyoffset + bandpixelheight * 0.05f), false));
        }
        g.setColour(Colour(0xffffffff));
        g.setFont(9.0f);
        // horizontal scale
        const int numFreqSteps = 10;
        const float freqScale = (getWidth() - 1)/(22050.0f);
        const float freqOffset = 22050.0f / numFreqSteps;
        for (int i = 0; i < numFreqSteps; ++i) {
            g.drawVerticalLine(freqScale * freqOffset * i, getHeight() - 5.0f, getHeight());
        }
        
        g.drawText(std::to_string(int(eqspec.getband(0).frequency)) + " Hz", 10, getHeight() - 20, 40, 20, Justification::left);
        g.drawText(std::to_string(int(eqspec.getband(numbands - 1).frequency)) + " Hz", getWidth() - 50, getHeight() - 20, 40, 20, Justification::right);
        
        const float horizScale = (getWidth() - 1);
        const float horizScaleOffset = horizScale / numFreqSteps;
        for (int i = 2; i < numFreqSteps; i += 2) {
            const float pos = horizScaleOffset * i;
            int band = pos * weight * numbands;
            g.drawText((std::to_string(int(eqspec.getband(band).frequency)) + std::string(" Hz")).c_str(), pos - 40, getHeight() - 20, 80, 20, Justification::centred);
        }
        
    } else {
        int bandoffset = std::max(1, int(100.0f * rms.size() / getWidth()));
        for (int i = 0; i < rms.size(); ++i) {
            //const auto bandyoffset = float(bands.size() - 1 - i) * bandpixelheight / bands.size();
            //const auto bandyoffset = (1.0 - 4.0 * log(1.0 * rms[i] + 1.0)) * bandpixelheight;
            //const auto bandyoffset = (1.0 - (0.015 * Decibels::gainToDecibels(rms[i] * rmsscale) + 0.75)) * bandpixelheight;
			const auto bandyoffset = -dbScale * (Decibels::gainToDecibels(rms[i] * rmsscale) - dBmax);
			const auto bandheight = bandpixelheight - bandyoffset;
            //const auto bandheight =(rms.size() + 1.0f) * bandpixelheight / rms.size();
            //g.setColour(Colour( 0.75f * float(i) / bands.size(), 1.0f, 1.0f, 1.0f));
            //g.setColour(Colour( 0.75f * float(i) / rms.size(), gradientsatoffset + gradientsatscale * std::abs(rmsgradient[i]), gradientvaloffset + gradientvalscale * std::abs(rmsgradient[i]), 1.0f));
            //g.setGradientFill(gradient);
            g.setFillType(FillType(Colour( 0.75f * float(i) / rms.size(), gradientsatoffset + gradientsatscale * std::abs(rmsgradient[i]), gradientvaloffset + gradientvalscale * std::abs(rmsgradient[i]), 1.0f)));
            
            
            //g.drawLine(i * bandpixeloffset + bandpixelwidth * 0.5f, bandyoffset, i * bandpixeloffset + bandpixelwidth * 0.5f, bandyoffset + bandheight, bandpixelwidth);
            g.fillRect(i * bandpixeloffset, bandyoffset, bandpixelwidth, bandheight);
            
            //g.drawVerticalLine(i * bandpixeloffset + bandpixelwidth * 0.5f, bandyoffset, bandyoffset + bandheight);
            
            //g.fillRect(Rectangle<float>(i * bandpixeloffset, bandyoffset, bandpixeloffset * 1.00f, bandheight));
            
            
            
            //bands[i]->setRectangle(Rectangle<float> (i * bandpixeloffset, bandyoffset, bandpixelwidth, bandheight));
            //bands[i]->setRectangle(Rectangle<float> (i * bandpixeloffset, bandyoffset, bandpixelwidth, bandheight));
            //gradient.point1 = Point<float>(0.0f, bandpixelheight);
            //gradient.point2 = Point<float>(0.0f, 0.0f);
            //bands[i]->setFill(gradient);
            //bands[i]->setFill(ColourGradient(Colour(0.333f, 0.5f, 1.0f, 1.0f), Point<float>(0.0f, bandyoffset), Colour(0.5f, 1.0f, 0.0f, 1.0f), Point<float>(0.0f, bandyoffset + bandpixelheight * 0.05f), false));
        }
        
        for (int i = 0; i < rms.size(); ++i) {
            if (i % bandoffset == 0) {
                g.setColour(Colour(0xffffffff));
                g.drawVerticalLine(i * bandpixeloffset + bandpixelwidth * 0.5f, getHeight() - 5.0f, getHeight());
                
                g.setFont(9.0f);
                g.drawText((std::to_string(int(eqspec.getband(i).frequency)) + std::string(" Hz")).c_str(), i * bandpixeloffset + bandpixelwidth * 0.5f - 40, getHeight() - 20, 80, 20, Justification::centred);
            }
        }
        
    }
    
    // vertical scale
    g.setColour(Colour(0xffffffff));
    const int numdBSteps = 8;
    for (int i = 0; i < numdBSteps; ++i) {
        g.drawHorizontalLine(dbScale * 10.0f * (i + 1), 0.0f, 5.0f);
    }
    g.drawHorizontalLine(dbScale * 10.0f, 0.0f, 15.0f);
    g.setFont(9.0f);
	g.drawText("0 dB", 25, dbScale * 10.0f - 10, 40, 20, Justification::left);
    g.drawText("-20 dB", 15, dbScale * 30.0f - 11, 40, 20, Justification::left);
    //g.drawText("-10 dB", 15, dbScale * 40.0f - 10, 40, 20, Justification::left);
    g.drawText("-40 dB", 15, dbScale * 50.0f - 11, 40, 20, Justification::left);
    //g.drawText("-30 dB", 15, dbScale * 60.0f - 10, 40, 20, Justification::left);
    g.drawText("-60 dB", 15, dbScale * 70.0f - 11, 40, 20, Justification::left);
}

void SpectrumGraphComponent::resized() {
    //updatebars();
}

void SpectrumGraphComponent::setnumbands(int numbands) {
	{
		const SpinLock::ScopedLockType sl(bufferlock);
		int eqtype = static_cast<int>(eq->eqtype());
		eqspec = eq->eqspec();
		//int octavedenom = eqtype > 0 ? (3 << (eqtype - 1)) : 1;
		//rmsscale = octavedenom;
		rmsscale = EQCalibration[eqtype];

		rms.resize(numbands, 0.0f);
		rmsgradient.resize(numbands, 0.0f);
	}
    resized();
	repaint();
}

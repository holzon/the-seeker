/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "EQOptimizedProcessor/EQOptimizedProcessor.h"

//==============================================================================
TheSeekerAudioProcessorEditor::TheSeekerAudioProcessorEditor (TheSeekerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), margin(4), spectrumgraph(std::static_pointer_cast<EQProcessor>(processor.eqprocessor), p.numbands()), uilayer(processor)
{
    lookAndFeel.setColourScheme (TheSeekerLookAndFeel::getDarkColourScheme());
    LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);
    
    openGLContext.attachTo (*getTopLevelComponent());
    
    openGLContext.setMultisamplingEnabled(true);
    OpenGLPixelFormat format;
    format.multisamplingLevel = 4;
    openGLContext.setPixelFormat(format);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(spectrumgraph);
   // addAndMakeVisible(eqcontrol);
    addAndMakeVisible(uilayer);
    spectrumgraph.toBack();
    //uilayer.toFront(true);
    setResizable (true, true);
    setSize (960, 360);

    
}

TheSeekerAudioProcessorEditor::~TheSeekerAudioProcessorEditor()
{
    openGLContext.detach();
}

//==============================================================================
void TheSeekerAudioProcessorEditor::paint (Graphics& g)
{
    StringArray renderingEngines;
    if (auto* peer = getPeer())
        renderingEngines = peer->getAvailableRenderingEngines();
    
    String no1 = renderingEngines[0];
    String no2 = renderingEngines[1];
    
    std::cout << "helloooo" << std::endl;
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colour (0xff080808));
}

void TheSeekerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    spectrumgraph.setBounds(margin, margin, getWidth() - 2 * margin, getHeight() - 2 * margin);
    //eqcontrol.setBounds(margin, margin, getWidth() - 2 * margin, getHeight() - 2 * margin);
    uilayer.setBounds(getLocalBounds());
}

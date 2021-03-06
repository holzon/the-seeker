/*
  ==============================================================================

    LabsLogo.cpp
    Created: 4 Oct 2017 11:14:25am
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SettingsPanel.h"

//==============================================================================
SettingsPanel::SettingsPanel(TheSeekerAudioProcessor& processor) :
processor(processor),
octaveselect(processor),
rmsslider(processor),
levelslider(processor),
Button("")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    Path hide_triangle_path;
    hide_triangle_path.addTriangle(2, 0, 10, 5, 2, 10);
    hide_triangle.setPath(hide_triangle_path);
    hide_triangle.setFill(Colour(255, 255, 255));

    Path show_triangle_path;
    show_triangle_path.addTriangle(8, 0, 0, 5, 8, 10);
    show_triangle.setPath(show_triangle_path);
    show_triangle.setFill(Colour(255, 255, 255));

    addChildComponent(levelslider);
    addChildComponent(octaveselect);
    addChildComponent(rmsslider);
    addChildComponent(hide_triangle);
    addChildComponent(show_triangle);
    updateVisibleState();
	setClickingTogglesState(true);

	attachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(processor.data(), "showsettings", *this));
}

SettingsPanel::~SettingsPanel()
{
}

void SettingsPanel::paint (Graphics& g)
{

}

void SettingsPanel::paintButton(Graphics& g,
	bool isMouseOverButton,
	bool isButtonDown) {

}

void SettingsPanel::clicked()
{
	updateVisibleState();
}

void SettingsPanel::updateVisibleState()
{
	visible = getToggleState();
	levelslider.setVisible(visible);
    octaveselect.setVisible(visible);
    rmsslider.setVisible(visible);
    hide_triangle.setVisible(visible);
    show_triangle.setVisible(!visible);
}

void SettingsPanel::resized()
{
    auto completearea (getLocalBounds());
	auto area = (getLocalBounds().reduced(12));

    int numitems = 3;
	int itempadding = 4;
    int itemheight = numitems * 28 + (numitems - 1) * itempadding;
    int itemoffset = (area.getHeight() - itemheight) * 0.5f;

    // offset items
	area.removeFromTop(itemoffset);
    auto rightpanel2 (area.removeFromTop(28));
	area.removeFromTop(itempadding);
    auto rightpanel3 (area.removeFromTop(28));
	area.removeFromTop(itempadding);
    auto rightpanel4 (area.removeFromTop(28));

	levelslider.setBounds(rightpanel2);
    rmsslider.setBounds(rightpanel3);
    octaveselect.setBounds(rightpanel4);

    int triangle_size = 10;
    int triangle_padding = 2;
    //triangle.setTransformToFit(Rectangle<float>(triangle_size, triangle_size), RectanglePlacement::xMid | RectanglePlacement::yMid);
    int triangle_size_y = (completearea.getHeight() + triangle_size) * 0.5f;
    int triangle_size_x = triangle_size + triangle_padding;
    int triangle_offset_y = (completearea.getHeight() - triangle_size) * 0.5f;
    int triangle_offset_x = completearea.getWidth() - triangle_size - triangle_padding;
    auto triangle_bounds = completearea.removeFromBottom(triangle_size_y).removeFromRight(triangle_size_x);
    hide_triangle.setBounds(completearea);
    hide_triangle.setOriginWithOriginalSize(Point<float>(triangle_offset_x, triangle_offset_y));
    show_triangle.setBounds(completearea);
    show_triangle.setOriginWithOriginalSize(Point<float>(triangle_offset_x, triangle_offset_y));
	updateVisibleState();
}

/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin editor.

==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProjectAudioProcessorEditor::ProjectAudioProcessorEditor(ProjectAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);

	// Combobox Magnitude
	addAndMakeVisible(magnitude_mode);
	magnitude_mode.addItemList(
		{
			"Keep Magnitude 1",
			"Keep Magnitude 2" ,
			"Add Both Magnitudes",
			"Multiply Both Magnitudes",
			"Mask Sound Track"
		}
	, 1);
	magnitude_mode.addListener(this);
	magnitude_mode.setSelectedId(1);
	magnitude_mode.setBounds((int)(0.05f * getWidth()), (int)(0.04f*getHeight()), (int)(getWidth()*0.4f), 20);

	// Combobox Phase
	addAndMakeVisible(phase_mode);
	phase_mode.addItemList(
		{
			"Keep Phase 1",
			"Keep Phase 2",
			"Add Both Phase"
		}
	, 1);
	phase_mode.addListener(this);
	phase_mode.setSelectedId(1);
	phase_mode.setBounds((int)(0.5f * getWidth()), (int)(0.04f*getHeight()), (int)(getWidth()*0.4f), 20);

	// Slider initialization
	sliderAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState, "Threshold", thresholdSlider);
	addAndMakeVisible(thresholdSlider);
	thresholdSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	thresholdSlider.setTextBoxStyle(Slider::TextBoxAbove, true, 100, 25);
	thresholdSlider.setTextValueSuffix(" db");
	thresholdSlider.setRange(-50.0f, 50.0f);
	thresholdSlider.setValue(100);
	thresholdSlider.setColour(Slider::ColourIds::thumbColourId, Colour::fromRGB(150, 0, 175));
	thresholdSlider.setColour(Slider::ColourIds::trackColourId, Colour::fromRGB(100, 0, 125));
	thresholdSlider.addListener(this);
	thresholdSlider.setBounds((int)((float)getWidth() * 0.3f), (int)(0.2 * getHeight()),
		(int)((float)getWidth() * 0.5f), (int)((float)getHeight() * 0.2f));
	// Add a label around slider
	addAndMakeVisible(thresholdLabel);
	thresholdSlider.addChildAndSetID(&thresholdLabel, "Threshold Label");
	thresholdLabel.setText("Mask Threshold", dontSendNotification);
	thresholdLabel.attachToComponent(&thresholdSlider, true);
}

ProjectAudioProcessorEditor::~ProjectAudioProcessorEditor()
{
}

//==============================================================================
void ProjectAudioProcessorEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::white);
	g.setFont(15.0f);
	g.drawFittedText("This is Phase Vocoder Project!", getLocalBounds(), Justification::centredBottom, 20);
}

void ProjectAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}

void ProjectAudioProcessorEditor::PhaseVocoderChanged()
{

}


void ProjectAudioProcessorEditor::buttonClicked(Button * toggleButton)
{
	
}


void ProjectAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
	///*This is where the slider changes the variable*/

	if (slider == &thresholdSlider)
	{
		target_val = thresholdSlider.getValue();
		PhaseVocoderChannel::setThreshold(target_val);
	}
}


void ProjectAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	auto selected_id = magnitude_mode.getSelectedId();

	/*This is where the slider changes the variable*/
	if (comboBoxThatHasChanged == &magnitude_mode) {
		// 1: Keep mag1 2: Keep mag2 3: Add mag1 and mag2
		// 4: Multiply mag1 and mag2 5: Mask
		PhaseVocoderChannel::setMagMode(selected_id - 1);
		// Show the threshold sliders when Mask mode selected
		thresholdSlider.setVisible(selected_id == 5);
	}
	else if (comboBoxThatHasChanged == &phase_mode) {
		PhaseVocoderChannel::setPhaseMode(selected_id - 1);
	}
}


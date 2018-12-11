/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin editor.

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ProjectAudioProcessorEditor : public AudioProcessorEditor, public Slider::Listener, public ComboBox::Listener, public ToggleButton::Listener
{
public:
	ProjectAudioProcessorEditor(ProjectAudioProcessor&);
	~ProjectAudioProcessorEditor();
	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void PhaseVocoderChanged();
	void sliderValueChanged(Slider *slider) override;
	void comboBoxChanged(ComboBox *comboBox) override;
	void buttonClicked(Button *toggleButton) override;
	//void SetupSlider(uint32_t slider_idx);
	ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> sliderAttach;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	ProjectAudioProcessor & processor;

	const int m_hoizontal_padding = 50;
	const int m_vertical_top_padding = 20;
	const int m_vertical_bottom_padding = 50;
	const float m_font_title_size = 40.0f;
	float target_val;
	// JUCE Tutorial -- Slider

	ComboBox magnitude_mode;
	ComboBox phase_mode;
	Slider thresholdSlider;
	Label thresholdLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectAudioProcessorEditor)
};






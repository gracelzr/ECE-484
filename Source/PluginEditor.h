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
class ProjectAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener, public ComboBox::Listener,
    ToggleButton::Listener
{
public:
    ProjectAudioProcessorEditor (ProjectAudioProcessor&);
    ~ProjectAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void PhaseVocoderChanged();
    void sliderValueChanged (Slider *slider) override;   
    void buttonClicked(Button *toggleButton) override;
    void comboBoxChanged(ComboBox *comboBox) override;
    void SetupSoundTrack1(uint32_t slider_idx);
    void SetupSoundTrack2(uint32_t slider_idx);
    void SetFrequencyBinVisibility(bool);
    
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> sliderAttach;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ProjectAudioProcessor& processor;
    
    
    struct FreqBin 
    {
        Slider slider; 
        Label label;
        ToggleButton toggle;
        Slider range;
        Label range_label;
		ToggleButton range_toggle;
    };

    const int m_hoizontal_padding = 50;
    const int m_vertical_top_padding = 20;
    const int m_vertical_bottom_padding = 50;
    const float m_font_title_size = 40.0f; 
	// JUCE Tutorial -- Slider
	Slider phaseSlider;
	ComboBox m_phase_vocoder_type;
	FreqBin m_freq_bin[SLIDER_COUNT];
	Slider m_master_bin_shift;
	ToggleButton m_set_all_bins;
	ToggleButton m_set_all_ranges;
	Slider m_pitch_shift; 
	 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectAudioProcessorEditor)
};






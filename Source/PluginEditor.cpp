/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
ProjectAudioProcessorEditor::ProjectAudioProcessorEditor (ProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
	for (uint32_t i = 0; i < SLIDER_COUNT; i++)
	{
		SetupSoundTrack1(i);
		SetupSoundTrack2(i);
	}
     addAndMakeVisible(m_master_bin_shift);
     m_master_bin_shift.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
     m_master_bin_shift.setRange(0.1f, 10.0f);   
     m_master_bin_shift.setSkewFactorFromMidPoint(1.0f);
     m_master_bin_shift.setValue(2.0f);
     m_master_bin_shift.setBounds((int)((float)getWidth() * 0.3f), (int)((float)getHeight() * 0.15f),
        (int)((float)getWidth() * 0.25f), (int)((float)getHeight() * 0.05f));
     m_master_bin_shift.setColour(Slider::ColourIds::thumbColourId, Colour::fromRGB(0, 0, 0));
     m_master_bin_shift.setColour(Slider::ColourIds::trackColourId, Colour::fromRGB(255, 255, 255));
     m_master_bin_shift.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
     m_master_bin_shift.addListener(this);
     m_master_bin_shift.setVisible(false);

     addAndMakeVisible(m_set_all_bins);
     m_set_all_bins.setButtonText("Sound Track 1");
     m_set_all_bins.setBounds((int)((float)getWidth() * 0.15f), (int)((0.1f)* getHeight()),
        (int)((float)getWidth() * 0.25f), (int)((float)getHeight() * 0.05f));
     m_set_all_bins.addListener(this);

     addAndMakeVisible(m_set_all_ranges);
     m_set_all_ranges.setButtonText("Sound Track 2");
     m_set_all_ranges.setBounds((int)((float)getWidth() * 0.6f), (int)((0.1f)* getHeight()),
		(int)((float)getWidth() * 0.25f), (int)((float)getHeight() * 0.05f));
     m_set_all_ranges.addListener(this);

     addAndMakeVisible(m_phase_vocoder_type);
     m_phase_vocoder_type.addListener(this);
     m_phase_vocoder_type.setSelectedId(1);
     m_phase_vocoder_type.setBounds((int)(0.025f * getWidth()), (int)(0.025f*getHeight()), (int)(getWidth()*0.2f), 20);

     //slider initialization
     sliderAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState, "Phaser", phaseSlider);
     addAndMakeVisible(phaseSlider);
     phaseSlider.setSliderStyle(Slider::SliderStyle::Rotary);
     phaseSlider.setTextBoxStyle(Slider::TextBoxAbove, true, 100, 25);
     phaseSlider.setRange(0.0f, 1.0f);
     //phaseSlider.setValue(PhaseVocoder::m_effect);
     phaseSlider.setColour(Slider::ColourIds::thumbColourId, Colour::fromRGB(150, 0, 175));
     phaseSlider.setColour(Slider::ColourIds::trackColourId, Colour::fromRGB(100, 0, 125));
     phaseSlider.addListener(this);
     setSize (800, 600);   
}

ProjectAudioProcessorEditor::~ProjectAudioProcessorEditor()
{
}

//==============================================================================
void ProjectAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("This is Phase Vocoder Project!", getLocalBounds(), Justification::centredBottom, 20);
}

void ProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void ProjectAudioProcessorEditor::PhaseVocoderChanged()
{
    
}

void ProjectAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
	/*This is where the slider changes the variable*/
	if (slider == &m_master_bin_shift)
	{
		for (uint32_t i = 0; i < SLIDER_COUNT; i++)
		{
			if (m_freq_bin[i].toggle.getToggleState())
			{
				uint32_t low = 55 << i;
				uint32_t hi = 55 << (i + 1);
				if (hi > 22050)
				{
					hi = 22050;
				}
				uint32_t mid = (low + hi) / 2;
				double desired = (double)mid * m_master_bin_shift.getValue();
				m_freq_bin[i].slider.setValue(desired);
				m_freq_bin[i].range.setValue(desired);
			}
		}
	}
	else
	{
		for (uint32_t i = 0; i < SLIDER_COUNT; i++)
		{
			if (slider == &m_freq_bin[i].slider)
			{
				//PhaseVocoder::change_slider_val((float)m_freq_bin[i].slider.getValue(), i);
				double lower = m_freq_bin[i].slider.getValue();
				double upper = 22050.0 - m_freq_bin[i].slider.getValue();
				double max = (upper < lower) ? upper : lower;
				double min = 0.0;
				double mean;
				if (m_freq_bin[i].range.getValue() > max)
				{
					mean = (max) / 2;
				}
				else
				{
					mean = m_freq_bin[i].range.getValue();
				}
				m_freq_bin[i].range.setRange(min, max);
				m_freq_bin[i].range.setValue(mean, NotificationType::sendNotification);
				int lower_bound = (int)min;
				int upper_bound = (int)max;
				std::string label_str = std::to_string(lower_bound) + " - " + std::to_string(upper_bound) + " Hz";
				m_freq_bin[i].range_label.setText(label_str, NotificationType::sendNotification);

				break;
			}
			if (slider == &m_freq_bin[i].range)
			{
				//PhaseVocoder::change_range_val((float)m_freq_bin[i].range.getValue(), i);
				break;
			}
		}
	}
}

void ProjectAudioProcessorEditor::buttonClicked(Button *toggleButton)
{
	if (toggleButton == &m_set_all_bins)
	{
		for (uint32_t i = 0; i < SLIDER_COUNT; i++)
		{
			m_freq_bin[i].toggle.setToggleState(m_set_all_bins.getToggleState(), NotificationType::sendNotification);
		}
	}
	else if (toggleButton == &m_set_all_ranges)
	{
		for (uint32_t i = 0; i < SLIDER_COUNT; i++)
		{
			m_freq_bin[i].range_toggle.setToggleState(m_set_all_ranges.getToggleState(), NotificationType::sendNotification);
		}
	}
	else
	{
		for (uint32_t i = 0; i < SLIDER_COUNT; i++)
		{
			if (toggleButton == &m_freq_bin[i].toggle)
			{
				//PhaseVocoder::change_slider_en(m_freq_bin[i].toggle.getToggleState(), i);
				break;
			}
			if (toggleButton == &m_freq_bin[i].range_toggle)
			{
				//PhaseVocoder::change_range_en(m_freq_bin[i].range_toggle.getToggleState(), i);
				break;
			}
		}
	}
}

void ProjectAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	/*This is where the slider changes the variable*/
	if (comboBoxThatHasChanged == &m_phase_vocoder_type) {
		//std::cout << phaseSlider.getValue() << std::endl;
		switch (m_phase_vocoder_type.getSelectedId())
		{
		case 1:
			//PhaseVocoder::change_type(ProcessType::Robotization);
			phaseSlider.setVisible(false);
			SetFrequencyBinVisibility(false);
			m_pitch_shift.setVisible(false);
			m_set_all_ranges.setToggleState(false, NotificationType::sendNotification);
			m_set_all_bins.setToggleState(false, NotificationType::sendNotification);
			break;
		case 2:
			//PhaseVocoder::change_type(ProcessType::Whisperization);
			phaseSlider.setVisible(false);
			SetFrequencyBinVisibility(false);
			m_pitch_shift.setVisible(false);
			m_set_all_ranges.setToggleState(false, NotificationType::sendNotification);
			m_set_all_bins.setToggleState(false, NotificationType::sendNotification);
			break;
		case 3:
			//PhaseVocoder::change_type(ProcessType::PitchShift);
			phaseSlider.setVisible(false);
			SetFrequencyBinVisibility(false);
			m_pitch_shift.setVisible(true);
			m_set_all_ranges.setToggleState(false, NotificationType::sendNotification);
			m_set_all_bins.setToggleState(false, NotificationType::sendNotification);
			break;
		case 4:
			//PhaseVocoder::change_type(ProcessType::Phaser);
			phaseSlider.setVisible(true);
			SetFrequencyBinVisibility(false);
			m_pitch_shift.setVisible(false);
			m_set_all_ranges.setToggleState(false, NotificationType::sendNotification);
			m_set_all_bins.setToggleState(false, NotificationType::sendNotification);
			break;
		case 5:
			//PhaseVocoder::change_type(ProcessType::NoneDebug);
			phaseSlider.setVisible(false);
			SetFrequencyBinVisibility(false);
			m_pitch_shift.setVisible(false);
			m_set_all_ranges.setToggleState(false, NotificationType::sendNotification);
			m_set_all_bins.setToggleState(false, NotificationType::sendNotification);
			break;
		case 6:
			//PhaseVocoder::change_type(ProcessType::BinShift);
			phaseSlider.setVisible(false);
			SetFrequencyBinVisibility(true);
			m_pitch_shift.setVisible(false);
			m_set_all_ranges.setToggleState(true, NotificationType::sendNotification);
			m_set_all_bins.setToggleState(true, NotificationType::sendNotification);
			break;
		}
	}
}

void ProjectAudioProcessorEditor::SetupSoundTrack1(uint32_t slider_idx)
{
	uint32_t low = 55 << slider_idx;
	uint32_t hi = 55 << (slider_idx + 1);
	uint32_t mid = (low + hi) / 2;
	double desired = (double)mid * m_master_bin_shift.getValue();
	float slider_pos = (float)slider_idx * 0.10f + 0.30f;
	addAndMakeVisible(m_freq_bin[slider_idx].slider);
	m_freq_bin[slider_idx].slider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	m_freq_bin[slider_idx].slider.setRange(0.0f, 22050.0f);
	m_freq_bin[slider_idx].slider.setSkewFactorFromMidPoint(2000.0f);
	m_freq_bin[slider_idx].slider.setValue(desired);

	m_freq_bin[slider_idx].slider.setBounds((int)((float)getWidth() * 0.1f), (int)(slider_pos * getHeight()),
	(int)((float)getWidth() * 0.25f), (int)((float)getHeight() * 0.06125f));
 
	m_master_bin_shift.setColour(Slider::ColourIds::thumbColourId, Colour::fromRGB(0, 0, 0));
	m_master_bin_shift.setColour(Slider::ColourIds::trackColourId, Colour::fromRGB(255, 255, 255));
	m_freq_bin[slider_idx].slider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
	m_freq_bin[slider_idx].slider.addListener(this);
	// Setup label
	uint32_t lower_bound = 55;
	lower_bound <<= slider_idx;
	uint32_t upper_bound = lower_bound << 1;
	if (upper_bound > 22050)
	upper_bound = 22050;
	std::string label_str = std::to_string(lower_bound) + " - " + std::to_string(upper_bound) + " Hz";
	addAndMakeVisible(m_freq_bin[slider_idx].label);
	m_freq_bin[slider_idx].label.setFont(Font(18.0f, Font::italic));
	m_freq_bin[slider_idx].label.setText(label_str, dontSendNotification);
	m_freq_bin[slider_idx].label.setColour(Label::textColourId, Colours::whitesmoke);
	m_freq_bin[slider_idx].label.setJustificationType(Justification::topRight);
	m_freq_bin[slider_idx].label.setBounds((int)((float)getWidth() * 0.0f), (int)((slider_pos - 0.025f)* getHeight()),
	(int)((float)getWidth() * 0.25f), (int)((float)getHeight() * 0.03f));
	// Setup toggle box
	addAndMakeVisible(m_freq_bin[slider_idx].toggle);
	m_freq_bin[slider_idx].toggle.setBounds((int)((float)getWidth() * 0.05625f), (int)((slider_pos + 0.0125f)* getHeight()),
	(int)((float)getWidth() * 0.05f), (int)((float)getHeight() * 0.05f));
	m_freq_bin[slider_idx].toggle.addListener(this);
 
}

void ProjectAudioProcessorEditor::SetupSoundTrack2(uint32_t slider_idx)
{
	float slider_pos = (float)slider_idx * 0.10f + 0.30f;
	addAndMakeVisible(m_freq_bin[slider_idx].range);
	m_freq_bin[slider_idx].range.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	m_freq_bin[slider_idx].range.setRange(-2000.0f, 18100.0f);
	m_freq_bin[slider_idx].range.setSkewFactorFromMidPoint(0.0f);
	m_freq_bin[slider_idx].range.setValue(m_freq_bin[slider_idx].range.getMaximum());
	m_freq_bin[slider_idx].range.setBounds((int)((float)getWidth() * 0.6f), (int)(slider_pos * getHeight()),
		(int)((float)getWidth() * 0.4f), (int)((float)getHeight() * 0.06125f));
	m_master_bin_shift.setColour(Slider::ColourIds::thumbColourId, Colour::fromRGB(0, 0, 0));
	m_master_bin_shift.setColour(Slider::ColourIds::trackColourId, Colour::fromRGB(255, 255, 255));
	m_freq_bin[slider_idx].range.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
	m_freq_bin[slider_idx].range.addListener(this);
	// Setup label
	int lower_bound = -2000;
	int upper_bound = 18100;
	std::string label_str = std::to_string(lower_bound) + " - " + std::to_string(upper_bound) + " Hz";
	addAndMakeVisible(m_freq_bin[slider_idx].range_label);
	m_freq_bin[slider_idx].range_label.setFont(Font(18.0f, Font::italic));
	m_freq_bin[slider_idx].range_label.setText(label_str, dontSendNotification);
	m_freq_bin[slider_idx].range_label.setColour(Label::textColourId, Colours::whitesmoke);
	m_freq_bin[slider_idx].range_label.setJustificationType(Justification::topRight);
	m_freq_bin[slider_idx].range_label.setBounds((int)((float)getWidth() * 0.55f), (int)((slider_pos - 0.025f)* getHeight()),
		(int)((float)getWidth() * 0.25f), (int)((float)getHeight() * 0.03f));
	// Setup toggle box
	addAndMakeVisible(m_freq_bin[slider_idx].range_toggle);
	m_freq_bin[slider_idx].range_toggle.setBounds((int)((float)getWidth() * 0.6), (int)((slider_pos + 0.0125f)* getHeight()),
		(int)((float)getWidth() * 0.05f), (int)((float)getHeight() * 0.05f));
	m_freq_bin[slider_idx].range_toggle.addListener(this);
}
void ProjectAudioProcessorEditor::SetFrequencyBinVisibility(bool vis)
{
	m_set_all_bins.setVisible(vis);
	m_set_all_ranges.setVisible(vis);
	m_master_bin_shift.setVisible(vis);
	for (uint32_t i = 0; i < SLIDER_COUNT; i++)
	{
		m_freq_bin[i].label.setVisible(vis);
		m_freq_bin[i].toggle.setVisible(vis);
		m_freq_bin[i].slider.setVisible(vis);
		m_freq_bin[i].range.setVisible(vis);		m_freq_bin[i].range_label.setVisible(vis);
		m_freq_bin[i].range_toggle.setVisible(vis);
	}
}

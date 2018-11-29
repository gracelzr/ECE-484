/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin editor.

==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

float desired;

//==============================================================================
ProjectAudioProcessorEditor::ProjectAudioProcessorEditor(ProjectAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);

	pv = new PhaseVocoder();


	//combobox Magniude
	addAndMakeVisible(magnitude_mode);
	magnitude_mode.addItem("Keep Magnitude 1", 1);
	magnitude_mode.addItem("Keep Magnitude 2", 2);
	magnitude_mode.addItem("Add Both Magnitudes", 3);
	magnitude_mode.addItem("Multiply Both Magnitudes", 4);
	magnitude_mode.addItem("Mask Sound Track", 5);
	magnitude_mode.addListener(this);
	magnitude_mode.setSelectedId(1);
	magnitude_mode.setBounds((int)(0.05f * getWidth()), (int)(0.04f*getHeight()), (int)(getWidth()*0.3f), 20);

	//combobox Phase
	addAndMakeVisible(phase_mode);
	phase_mode.addItem("Keep Phase 1", 1);
	phase_mode.addItem("Keep Phase 2", 2);
	phase_mode.addItem("Add Both Phase", 3);
	//phase_mode.addItem("Multiply Both Phase", 4);
	phase_mode.addListener(this);
	phase_mode.setSelectedId(1);
	phase_mode.setBounds((int)(0.4f * getWidth()), (int)(0.04f*getHeight()), (int)(getWidth()*0.3f), 20);

	//slider initialization
	sliderAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState, "Threshold", thresholdSlider);
	addAndMakeVisible(thresholdSlider);
	thresholdSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	thresholdSlider.setTextBoxStyle(Slider::TextBoxAbove, true, 100, 25);
	thresholdSlider.setRange(-50.0f, 50.0f);
	thresholdSlider.setValue(100);
	thresholdSlider.setColour(Slider::ColourIds::thumbColourId, Colour::fromRGB(150, 0, 175));
	thresholdSlider.setColour(Slider::ColourIds::trackColourId, Colour::fromRGB(100, 0, 125));
	thresholdSlider.addListener(this);	thresholdSlider.setBounds((int)((float)getWidth() * 0.1f), (int)(0.2 * getHeight()),
		(int)((float)getWidth() * 0.5f), (int)((float)getHeight() * 0.2f));
	setSize(400, 300);
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

//void ProjectAudioProcessorEditor::SetupSlider(uint32_t slider_idx)
//{
//	uint32_t low = 55 << slider_idx;
//	uint32_t hi = 55 << (slider_idx + 1);
//	uint32_t mid = (low + hi) / 2;
//	double desired = (double)mid * thresholdSlider.getValue();
//	float slider_pos = (float)slider_idx * 0.10f + 0.10f;
//	addAndMakeVisible(thresholdSlider);
//	thresholdSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
//	thresholdSlider.setRange(0.0f, 22050.0f);
//	thresholdSlider.setSkewFactorFromMidPoint(2000.0f);
//	thresholdSlider.setValue(desired);
//	thresholdSlider.setBounds((int)((float)getWidth() * 0.05f), (int)(slider_pos * getHeight()),
//		(int)((float)getWidth() * 0.5f), (int)((float)getHeight() * 0.06125f));
//	thresholdSlider.setColour(Slider::ColourIds::thumbColourId, Colour::fromRGB(0, 0, 0));
//	thresholdSlider.setColour(Slider::ColourIds::trackColourId, Colour::fromRGB(255, 255, 255));
//	thresholdSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
//	thresholdSlider.addListener(this);
//	// Setup label
//	uint32_t lower_bound = 55;
//	lower_bound <<= slider_idx;
//	uint32_t upper_bound = lower_bound << 1;
//	if (upper_bound > 22050)
//		upper_bound = 22050;
//	std::string label_str = std::to_string(lower_bound) + " - " + std::to_string(upper_bound) + " Hz";
//	addAndMakeVisible(thres_range);
//	thres_range.setFont(Font(18.0f, Font::italic));
//	thres_range.setText(label_str, dontSendNotification);
//	thres_range.setColour(Label::textColourId, Colours::whitesmoke);
//	thres_range.setJustificationType(Justification::topRight);
//	thres_range.setBounds((int)((float)getWidth() * 0.0f), (int)((slider_pos - 0.025f)* getHeight()),
//		(int)((float)getWidth() * 0.25f), (int)((float)getHeight() * 0.03f));
//	// Setup toggle box
//	addAndMakeVisible(toggle);
//	toggle.setBounds((int)((float)getWidth() * 0.05625f), (int)((slider_pos + 0.0125f)* getHeight()),
//		(int)((float)getWidth() * 0.05f), (int)((float)getHeight() * 0.05f));
//	toggle.addListener(this);
//
//}

void ProjectAudioProcessorEditor::buttonClicked(Button * toggleButton)
{
	
}

void ProjectAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
	///*This is where the slider changes the variable*/

	if (slider == &thresholdSlider)
	{
		//uint32_t high = thresholdSlider.getValue();
		//uint32_t low = 55 - thresholdSlider.getValue();
		//uint32_t mid = (low + high) / 2;
		desired = thresholdSlider.getValue();
		//thresholdSlider.setValue(desired);
		pv->getThreshold(desired);
		std::string label_str = std::to_string(desired) + " dB";
	}
}



void ProjectAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	/*This is where the slider changes the variable*/
	if (comboBoxThatHasChanged == &magnitude_mode) {
		std::cout << thresholdSlider.getValue() << std::endl;
		switch (magnitude_mode.getSelectedId())
		{
		case 1:
			//keep mag1
			pv->changeMagMode(0);
			thresholdSlider.setVisible(false);
			thres_range.setVisible(false);
			toggle.setVisible(false);

			break;
		case 2:
			//keep mag2
			pv->changeMagMode(1);
			thresholdSlider.setVisible(false);
			thres_range.setVisible(false);
			toggle.setVisible(false);
			break;
		case 3:
			//add mag1 and mag2
			pv->changeMagMode(2);
			thresholdSlider.setVisible(false);
			thres_range.setVisible(false);
			toggle.setVisible(false);
			break;
		case 4:
			//multiply mag1 and mag2
			pv->changeMagMode(3);
			thresholdSlider.setVisible(false);
			thres_range.setVisible(false);
			toggle.setVisible(false);
			break;
		case 5:
			//mask
			pv->changeMagMode(4);
			thresholdSlider.setVisible(true);
			thres_range.setVisible(true);
			toggle.setVisible(true);
			std::string label_str = std::to_string(desired) + " dB";
			break;
		}
	}
	if (comboBoxThatHasChanged == &phase_mode) {
		switch (phase_mode.getSelectedId())
		{
		case 1:
			pv->changePhaseMode(0);
			thresholdSlider.setVisible(false);
			thres_range.setVisible(false);
			toggle.setVisible(false);
			break;
		case 2:
			pv->changePhaseMode(1);
			thresholdSlider.setVisible(false);
			thres_range.setVisible(false);
			toggle.setVisible(false);
			break;
		case 3:
			pv->changePhaseMode(2);
			thresholdSlider.setVisible(false);
			thres_range.setVisible(false);
			toggle.setVisible(false);
			break;
		/*case 4:
			thresholdSlider.setVisible(false);
			thres_range.setVisible(false);
			toggle.setVisible(false);
			break;*/
		}
	}
}


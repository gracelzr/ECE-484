/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin processor.

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PV.h"
extern "C" {
#include "PV_core.h"
}
#define fftFrameSize 1024
#define osamp 5
#define SAMPLE_RATE 44100.0f

//==============================================================================
/**
*/
class ProjectAudioProcessor : public AudioProcessor
{
public:
	//==============================================================================
	ProjectAudioProcessor();
	~ProjectAudioProcessor();

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	PhaseVocoder * pv;
	dsp::ProcessorDuplicator<dsp::IIR::Filter< float >, dsp::IIR::Coefficients< float >> lowpass;
	void dsp_process(dsp::ProcessContextReplacing<float>);
	void update();

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

	//==============================================================================
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;
	AudioProcessorValueTreeState valueTreeState;
	//static void DSPThread(float* input, float* output, uint32_t buffer_size, uint32 channel, void* pv);
private:
	//==============================================================================

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectAudioProcessor)
};
/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin processor.

==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <thread>

//==============================================================================
ProjectAudioProcessor::ProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
		//.withInput("Input1", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::mono(), true)
#endif
	), valueTreeState(*this, nullptr)
#endif
{
	pv = new PhaseVocoder();
}


ProjectAudioProcessor::~ProjectAudioProcessor()
{
	if (pv)
	{
		delete pv;
		pv = NULL;
	}
}

//==============================================================================
const String ProjectAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool ProjectAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool ProjectAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool ProjectAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double ProjectAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int ProjectAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int ProjectAudioProcessor::getCurrentProgram()
{
	return 0;
}

void ProjectAudioProcessor::setCurrentProgram(int index)
{
	(void)index;
}

const String ProjectAudioProcessor::getProgramName(int index)
{
	(void)index;
	return {};
}

void ProjectAudioProcessor::changeProgramName(int index, const String& newName)
{
	(void)index;
	(void)newName;
}

//==============================================================================
void ProjectAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();
	lowpass.prepare(spec);
	lowpass.reset();
}

void ProjectAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ProjectAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void ProjectAudioProcessor::dsp_process(dsp::ProcessContextReplacing<float> context)
{
	(void)context;
}
void ProjectAudioProcessor::update()
{

}

void ProjectAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();
	(void)midiMessages;

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	// Make sure to reset the state if your inner loop is processing
	// the samples and the outer loop is handling the channels.
	// Alternatively, you can process the samples with the channels
	// interleaved by keeping the same state.
	auto numSample = buffer.getNumSamples();

	AudioBuffer<float> inbuf(2, numSample);

	inbuf.copyFrom(0, 0, buffer.getReadPointer(0), numSample);
	inbuf.copyFrom(1, 0, buffer.getReadPointer(1), numSample);

	auto* indata = inbuf.getReadPointer(0);	
	auto* indata1 = inbuf.getReadPointer(1);
	
	auto* outdata = buffer.getWritePointer(0);
	auto* outdata1 = buffer.getWritePointer(1);

	//PV(25, 2, 1, numSample, numSample, 2048, 4, 44100, indata, indata2, outdata);
	// ..do something to the data...
	pv->passParameters(numSample, numSample, fftFrameSize, osamp, SAMPLE_RATE, indata, indata1, outdata);
	//pv->passParameters(numSample, numSample, fftFrameSize, osamp, SAMPLE_RATE, indata, buffer.getReadPointer(2), outdata);
	//pv->passParameters(numSample, numSample, fftFrameSize, osamp, SAMPLE_RATE, indata1, buffer.getReadPointer(3), outdata1);

}

//==============================================================================
bool ProjectAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ProjectAudioProcessor::createEditor()
{
	return new ProjectAudioProcessorEditor(*this);
}

//==============================================================================
void ProjectAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
	(void)destData;
}

void ProjectAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
	(void)data;
	(void)sizeInBytes;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ProjectAudioProcessor();
}

/*
==============================================================================

PV.h
Created: 26 Nov 2018 1:51:23pm
Author:  grace

==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
extern "C" {
#include "PV_core.h"
}

class PhaseVocoderChannel
{

public:

	PhaseVocoderChannel();
	~PhaseVocoderChannel();

	static void setMagMode(uint8_t mag_mode);
	static void setPhaseMode(uint8_t phase_mode);
	static void setThreshold(int threshold);
	static void setFftFrameSize(uint64_t fft_frame_size);
	static void setOSAMP(uint64_t osamp);
	static void initParams(uint8_t mag_mode, uint8_t phase_mode, int threshold, uint64_t fft_frame_size, uint64_t osamp);
	static void setRuntimeParams(long sample_to_process, float sample_rate);

	void processAudioChunk(const float * indata, const float * indata2, float * outdata);
	void processAudioChunk2(const float * indata, const float * indata2, float * outdata);

private:
	static uint8_t magMode;
	static uint8_t phaseMode;
	static int thresh;
	static uint64_t fftFrameSize;
	static uint64_t osamp;
	static uint64_t numSampsToProcess;
	static float sampleRate;

	float gInFIFO[MAX_FRAME_LENGTH];
	float gInFIFO2[MAX_FRAME_LENGTH];

	float gFFTworksp[2 * MAX_FRAME_LENGTH];
	float gFFTworksp2[2 * MAX_FRAME_LENGTH];

	float gLastPhase[MAX_FRAME_LENGTH / 2 + 1];
	float gLastPhase2[MAX_FRAME_LENGTH / 2 + 1];

	float gAnaMagn[MAX_FRAME_LENGTH];
	float gAnaMagn2[MAX_FRAME_LENGTH];

	float gAnaFreq[MAX_FRAME_LENGTH];
	float gAnaFreq2[MAX_FRAME_LENGTH];

	float gOutFIFO[MAX_FRAME_LENGTH];
	float gSumPhase[MAX_FRAME_LENGTH / 2 + 1];
	float gOutputAccum[2 * MAX_FRAME_LENGTH];
	float gSynFreq[MAX_FRAME_LENGTH];
	float gSynMagn[MAX_FRAME_LENGTH];

	long gRover, gRover2;

	static void smbFft(float * fftBuffer, long fftFrameSize, long sign);
};
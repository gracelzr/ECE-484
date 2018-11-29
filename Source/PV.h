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
#define FFT_ORDER 11
#define FFT_SIZE (1 << FFT_ORDER)
#define WINDOW_SIZE FFT_SIZE
#define HOP_SIZE FFT_SIZE/2
#define SCALING_FACTOR (WINDOW_SIZE / (HOP_SIZE*2))
#define THREAD_COUNT 2
#define BUFFER_SIZE FFT_SIZE * 2

#define SAMPLE_RATE 44100.0f
#define FREQUENCY_MAX 22050


class PhaseVocoder
{



public:
	PhaseVocoder();
	~PhaseVocoder();

	void changeMagMode(int);
	void changePhaseMode(int);

	void getThreshold(int);
	void passParameters(long, long, long, long, float, const float *, const float *, float *);
private:


};
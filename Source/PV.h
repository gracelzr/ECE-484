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

class PhaseVocoder
{

public:
	static uint8_t MAGMODE;
	static uint8_t PHASEMODE;
	static uint8_t thresh;
	PhaseVocoder();
	~PhaseVocoder();

	void changeMagMode(uint8_t);
	void changePhaseMode(uint8_t);

	void getThreshold(int);
	void processAudioChunk(long, long, long, long, float, const float *, const float *, float *);
	void processAudioChunk2(long, long, long, long, float, const float *, const float *, float *);
private:


};
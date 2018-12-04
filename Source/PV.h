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
	PhaseVocoder();
	~PhaseVocoder();

	void changeMagMode(int);
	void changePhaseMode(int);

	void getThreshold(int);
	void passParameters(long, long, long, long, float, const float *, const float *, float *);
private:


};
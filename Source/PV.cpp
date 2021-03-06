/*
==============================================================================

PV.cpp
Created: 26 Nov 2018 1:51:16pm
Author:  grace

==============================================================================
*/

#include "PV.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime>
#include <iostream>
#include <bitset>
#include <chrono>
#include <math.h>
#include <fstream>
#include <iomanip>

int MAGMODE;
int PHASEMODE;
int thresh;

PhaseVocoder::PhaseVocoder()
{

}


PhaseVocoder::~PhaseVocoder()
{
}

void PhaseVocoder::changeMagMode(int mag_mode)
{
	MAGMODE = mag_mode;
}

void PhaseVocoder::changePhaseMode(int phase_mode)
{
	PHASEMODE = phase_mode;
}

void PhaseVocoder::getThreshold(int thre_input)
{
	thresh = thre_input;

}

void PhaseVocoder::passParameters(long numSampsToProcess, long numSamps2, long fftFrameSize, long osamp, float sampleRate, const float *indata, const float *indata2, float *outdata)
{
	PV(thresh, MAGMODE, PHASEMODE, numSampsToProcess, numSamps2, fftFrameSize, osamp, sampleRate, indata, indata2, outdata);
}

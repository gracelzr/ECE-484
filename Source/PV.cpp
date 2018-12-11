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

void PhaseVocoder::setMagMode(uint8_t mag_mode)
{
	PhaseVocoder::magMode = mag_mode;
}

void PhaseVocoder::setPhaseMode(uint8_t phase_mode)
{
	PhaseVocoder::phaseMode = phase_mode;
}

void PhaseVocoder::getThreshold(int thre_input)
{
	thresh = thre_input;

}

void PhaseVocoder::processAudioChunk(long numSampsToProcess, long numSamps2, long fftFrameSize, long osamp, float sampleRate, const float *indata, const float *indata2, float *outdata)
{
	PV(thresh, magMode, phaseMode, numSampsToProcess, numSamps2, fftFrameSize, osamp, sampleRate, indata, indata2, outdata);
}

void PhaseVocoder::processAudioChunk2(long numSampsToProcess, long numSamps2, long fftFrameSize, long osamp, float sampleRate, const float *indata, const float *indata2, float *outdata)
{
	PV2(thresh, magMode, phaseMode, numSampsToProcess, numSamps2, fftFrameSize, osamp, sampleRate, indata, indata2, outdata);
}
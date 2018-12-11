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

uint8_t PhaseVocoderChannel::magMode;
uint8_t PhaseVocoderChannel::phaseMode;
int PhaseVocoderChannel::thresh;
uint64_t PhaseVocoderChannel::fftFrameSize;
uint64_t PhaseVocoderChannel::osamp;
uint64_t PhaseVocoderChannel::numSampsToProcess;
float PhaseVocoderChannel::sampleRate;

PhaseVocoderChannel::PhaseVocoderChannel()
{
	// Initialize private buffers
	memset(gInFIFO, 0, MAX_FRAME_LENGTH * sizeof(float));
	memset(gInFIFO2, 0, MAX_FRAME_LENGTH * sizeof(float));
	memset(gOutFIFO, 0, MAX_FRAME_LENGTH * sizeof(float));
	memset(gFFTworksp, 0, 2 * MAX_FRAME_LENGTH * sizeof(float));
	memset(gFFTworksp2, 0, 2 * MAX_FRAME_LENGTH * sizeof(float));
	memset(gLastPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof(float));
	memset(gLastPhase2, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof(float));
	memset(gSumPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof(float));
	memset(gOutputAccum, 0, 2 * MAX_FRAME_LENGTH * sizeof(float));
	memset(gAnaFreq, 0, MAX_FRAME_LENGTH * sizeof(float));
	memset(gAnaMagn, 0, MAX_FRAME_LENGTH * sizeof(float));
	memset(gAnaFreq2, 0, MAX_FRAME_LENGTH * sizeof(float));
	memset(gAnaMagn2, 0, MAX_FRAME_LENGTH * sizeof(float));

	gRover = false;
	gRover2 = false;
}

PhaseVocoderChannel::~PhaseVocoderChannel()
{
	// So far no dynamic memory allocation
}

void PhaseVocoderChannel::setMagMode(uint8_t mag_mode)
{
	PhaseVocoderChannel::magMode = mag_mode;
}

void PhaseVocoderChannel::setPhaseMode(uint8_t phase_mode)
{
	PhaseVocoderChannel::phaseMode = phase_mode;
}

void PhaseVocoderChannel::setThreshold(int threshold)
{
	PhaseVocoderChannel::thresh = threshold;
}

void PhaseVocoderChannel::setFftFrameSize(uint64_t fft_frame_size)
{
	PhaseVocoderChannel::fftFrameSize = fft_frame_size;
}

void PhaseVocoderChannel::setOSAMP(uint64_t osamp_val)
{
	PhaseVocoderChannel::osamp = osamp_val;
}

void PhaseVocoderChannel::initParams(uint8_t mag_mode, uint8_t phase_mode, int threshold, uint64_t fft_frame_size, uint64_t osamp)
{
	setMagMode(mag_mode);
	setPhaseMode(phase_mode);
	setThreshold(threshold);
	setFftFrameSize(fft_frame_size);
	setOSAMP(osamp);
}

void PhaseVocoderChannel::setRuntimeParams(long sample_to_process, float sample_rate)
{
	PhaseVocoderChannel::numSampsToProcess = sample_to_process;
	PhaseVocoderChannel::sampleRate = sample_rate;
}

void PhaseVocoderChannel::processAudioChunk(const float *indata, const float *indata2, float *outdata)
{
	//PV(thresh, magMode, phaseMode, numSampsToProcess, numSampsToProcess, fftFrameSize, osamp, sampleRate, indata, indata2, outdata);

	double magn, phase, magn2, phase2, tmp, tmp2, window, real, imag, real2, imag2;
	double freqPerBin, expct;
	long i, k, j = 0, qpd, qpd2, index, inFifoLatency, stepSize, fftFrameSize2;

	/* set up some handy variables */
	fftFrameSize2 = fftFrameSize / 2;
	stepSize = fftFrameSize / osamp;
	freqPerBin = sampleRate / (double)fftFrameSize;
	expct = 2.*M_PI*(double)stepSize / (double)fftFrameSize;
	inFifoLatency = fftFrameSize - stepSize;
	if (gRover == false) gRover = inFifoLatency;
	//second rover counter
	if (gRover2 == false) gRover2 = inFifoLatency;

	/* main processing loop */
	for (i = 0; i < numSampsToProcess; i++) {
		/* As long as we have not yet collected enough data just read in */
		gInFIFO[gRover] = indata[i];
		//If the second sound is shorter than the main sound, loop it
		//if (j >= numSamps2) {
		//	j = 0;
		//}
		gInFIFO2[gRover2] = indata2[j];
		outdata[i] = gOutFIFO[gRover - inFifoLatency];
		gRover++;
		gRover2++;
		j++;

		/* now we have enough data for processing */
		if (gRover >= fftFrameSize) {
			gRover = inFifoLatency;
			gRover2 = inFifoLatency;


			/* do windowing and re,im interleave */
			for (k = 0; k < fftFrameSize; k++) {
				window = -.5*cos(2.*M_PI*(double)k / (double)fftFrameSize) + .5;
				gFFTworksp[2 * k] = gInFIFO[k] * window;
				gFFTworksp[2 * k + 1] = 0.;
				//Windowing second file
				gFFTworksp2[2 * k] = gInFIFO2[k] * window;
				gFFTworksp2[2 * k + 1] = 0.;
			}


			/* ***************** ANALYSIS ******************* */
			/* do transform */
			smbFft(gFFTworksp, fftFrameSize, -1);
			smbFft(gFFTworksp2, fftFrameSize, -1);

			/* this is the analysis step */
			for (k = 0; k <= fftFrameSize2; k++) {
				/* de-interlace FFT buffer */
				real = gFFTworksp[2 * k];
				imag = gFFTworksp[2 * k + 1];
				//Same for second sound
				real2 = gFFTworksp2[2 * k];
				imag2 = gFFTworksp2[2 * k + 1];

				/* compute magnitude and phase */
				magn = 2.*sqrt(real*real + imag * imag);
				phase = atan2(imag, real);
				//Same for second sound
				magn2 = 2.*sqrt(real2*real2 + imag2 * imag2);
				phase2 = atan2(imag2, real2);

				/* compute phase difference */
				tmp = phase - gLastPhase[k];
				gLastPhase[k] = phase;
				//Same for second sound
				tmp2 = phase2 - gLastPhase2[k];
				gLastPhase2[k] = phase2;

				/* subtract expected phase difference */
				tmp -= (double)k*expct;
				tmp2 -= (double)k*expct;

				/* map delta phase into +/- Pi interval */
				qpd = tmp / M_PI;
				if (qpd >= 0) qpd += qpd & 1;
				else qpd -= qpd & 1;
				tmp -= M_PI * (double)qpd;
				//Same for second sound
				qpd2 = tmp2 / M_PI;
				if (qpd2 >= 0) qpd2 += qpd2 & 1;
				else qpd2 -= qpd2 & 1;
				tmp2 -= M_PI * (double)qpd2;


				/* get deviation from bin frequency from the +/- Pi interval */
				tmp = osamp * tmp / (2.*M_PI);
				tmp2 = osamp * tmp2 / (2.*M_PI);

				/* compute the k-th partials' true frequency */
				tmp = (double)k*freqPerBin + tmp * freqPerBin;
				tmp2 = (double)k*freqPerBin + tmp2 * freqPerBin;

				/* store magnitude and true frequency in analysis arrays */
				gAnaMagn[k] = magn;
				gAnaFreq[k] = tmp;
				gAnaMagn2[k] = magn2;
				gAnaFreq2[k] = tmp2;


			}

			/* ***************** PROCESSING ******************* */
			/* this does the actual pitch shifting */
			memset(gSynMagn, 0, fftFrameSize * sizeof(float));
			memset(gSynFreq, 0, fftFrameSize * sizeof(float));
			for (k = 0; k <= fftFrameSize2; k++) {
				//original code
				/*index = k*pitchShift;
				if (index <= fftFrameSize2) {
				gSynMagn[index] += gAnaMagn[k];
				gSynFreq[index] = gAnaFreq[k] * pitchShift;
				} */

				//new code
				if (magMode == 2) {
					//Add Magnitudes
					gSynMagn[k] = gAnaMagn[k] + gAnaMagn2[k];
				}
				else if (magMode == 3) {
					//Multiply Magnitudes
					gSynMagn[k] = gAnaMagn[k] * gAnaMagn2[k];
				}
				else if (magMode == 4) {
					//Mask original magnitude with second magnitude if less than thresh
					if (gAnaMagn[k] < thresh) {
						gSynMagn[k] = gAnaMagn2[k];
					}
					else {
						gSynMagn[k] = gAnaMagn[k];
					}
				}
				else if (magMode == 1) {
					//Keep second magnitude
					gSynMagn[k] = gAnaMagn2[k];
				}
				else if (magMode == 0) {
					//Keep original magnitude
					gSynMagn[k] = gAnaMagn[k];
				}

				if (phaseMode == 2) {
					//Add Phases
					gSynFreq[k] = gAnaFreq[k] + gAnaFreq2[k];
				}
				else if (phaseMode == 1) {
					//Keep second Phase
					gSynFreq[k] = gAnaFreq2[k];
				}
				else if (phaseMode == 0) {
					//Keep original Phase
					gSynFreq[k] = gAnaFreq[k];
				}
			}

			/* ***************** SYNTHESIS ******************* */
			/* this is the synthesis step */
			for (k = 0; k <= fftFrameSize2; k++) {


				/* get magnitude and true frequency from synthesis arrays */
				magn = gSynMagn[k];
				tmp = gSynFreq[k];


				/* subtract bin mid frequency */
				tmp -= (double)k*freqPerBin;


				/* get bin deviation from freq deviation */
				tmp /= freqPerBin;


				/* take OSAMP into account */
				tmp = 2.*M_PI*tmp / osamp;


				/* add the overlap phase advance back in */
				tmp += (double)k*expct;


				/* accumulate delta phase to get bin phase */
				gSumPhase[k] += tmp;
				phase = gSumPhase[k];


				/* get real and imag part and re-interleave */
				gFFTworksp[2 * k] = magn * cos(phase);
				gFFTworksp[2 * k + 1] = magn * sin(phase);
			}


			/* zero negative frequencies */
			for (k = fftFrameSize + 2; k < 2 * fftFrameSize; k++) gFFTworksp[k] = 0.;


			/* do inverse transform */
			smbFft(gFFTworksp, fftFrameSize, 1);


			/* do windowing and add to output accumulator */
			for (k = 0; k < fftFrameSize; k++) {
				window = -.5*cos(2.*M_PI*(double)k / (double)fftFrameSize) + .5;
				gOutputAccum[k] += 2.*window*gFFTworksp[2 * k] / (fftFrameSize2*osamp);
			}
			for (k = 0; k < stepSize; k++) gOutFIFO[k] = gOutputAccum[k];


			/* shift accumulator */
			memmove(gOutputAccum, gOutputAccum + stepSize, fftFrameSize * sizeof(float));


			/* move input FIFO */
			for (k = 0; k < inFifoLatency; k++) gInFIFO[k] = gInFIFO[k + stepSize];
			//Same for second sound
			for (k = 0; k < inFifoLatency; k++) gInFIFO2[k] = gInFIFO2[k + stepSize];
		}
	}
}


void PhaseVocoderChannel::processAudioChunk2(const float *indata, const float *indata2, float *outdata)
{
	PV2(thresh, magMode, phaseMode, numSampsToProcess, numSampsToProcess, fftFrameSize, osamp, sampleRate, indata, indata2, outdata);
}

void PhaseVocoderChannel::smbFft(float *fftBuffer, long fftFrameSize, long sign)
/*
FFT routine, (C)1996 S.M.Bernsee. Sign = -1 is FFT, 1 is iFFT (inverse)
Fills fftBuffer[0...2*FFT_FRAME_SIZE-1] with the Fourier transform of the
time domain data in fftBuffer[0...2*FFT_FRAME_SIZE-1]. The FFT array takes
and returns the cosine and sine parts in an interleaved manner, ie.
fftBuffer[0] = cosPart[0], fftBuffer[1] = sinPart[0], asf. FFT_FRAME_SIZE
must be a power of 2. It expects a complex input signal (see footnote 2),
ie. when working with 'common' audio signals our input signal has to be
passed as {in[0],0.,in[1],0.,in[2],0.,...} asf. In that case, the transform
of the frequencies of interest is in fftBuffer[0...FFT_FRAME_SIZE].
*/
{
	float wr, wi, arg, *p1, *p2, temp;
	float tr, ti, ur, ui, *p1r, *p1i, *p2r, *p2i;
	long i, bitm, j, le, le2, k;


	for (i = 2; i < 2 * fftFrameSize - 2; i += 2) {
		for (bitm = 2, j = 0; bitm < 2 * fftFrameSize; bitm <<= 1) {
			if (i & bitm) j++;
			j <<= 1;
		}
		if (i < j) {
			p1 = fftBuffer + i; p2 = fftBuffer + j;
			temp = *p1; *(p1++) = *p2;
			*(p2++) = temp; temp = *p1;
			*p1 = *p2; *p2 = temp;
		}
	}
	for (k = 0, le = 2; k < (long)(log(fftFrameSize) / log(2.) + .5); k++) {
		le <<= 1;
		le2 = le >> 1;
		ur = 1.0;
		ui = 0.0;
		arg = M_PI / (le2 >> 1);
		wr = cos(arg);
		wi = sign * sin(arg);
		for (j = 0; j < le2; j += 2) {
			p1r = fftBuffer + j; p1i = p1r + 1;
			p2r = p1r + le2; p2i = p2r + 1;
			for (i = j; i < 2 * fftFrameSize; i += le) {
				tr = *p2r * ur - *p2i * ui;
				ti = *p2r * ui + *p2i * ur;
				*p2r = *p1r - tr; *p2i = *p1i - ti;
				*p1r += tr; *p1i += ti;
				p1r += le; p1i += le;
				p2r += le; p2i += le;
			}
			tr = ur * wr - ui * wi;
			ui = ur * wi + ui * wr;
			ur = tr;
		}
	}
}
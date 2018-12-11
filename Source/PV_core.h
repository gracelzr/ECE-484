#pragma once
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>


#define M_PI 3.14159265358979323846
#define MAX_FRAME_LENGTH 8192

void smbFft(float *, long , long );
void PV(float, int, int, long, long, long, long, float, const float *, const float *, float *);
void PV2(float, int, int, long, long, long, long, float, const float *, const float *, float *);


#ifndef DEFINITIONS_CUH
#define DEFINITIONS_CUH 1

#include <stdio.h>
#include <math.h>

#define MAX_TRACKS 10000
#define TRACK_SIZE 24

#define MAX_POST_TRACKS 100
#define REQUIRED_UNIQUES 0.67f
#define MIN_HITS_TRACK 3

#define PARAM_W 0.0144338f // 0.050 / sqrt( 12. )
#define PARAM_MAXXSLOPE 0.4f
#define PARAM_MAXYSLOPE 0.3f

#define PARAM_TOLERANCE_BASIC 0.15f
#define PARAM_TOLERANCE_EXTENDED 0.3f

#define PARAM_MAXCHI2_BASIC 100.0f
#define PARAM_MAXCHI2_EXTENDED 200.0f

#define PARAM_TOLERANCE PARAM_TOLERANCE_EXTENDED
#define PARAM_MAXCHI2 PARAM_MAXCHI2_EXTENDED


#define S_ROWS 2
#define S_COLS 2
#define F_ROWS 4
#define F_COLS 4
#define H_ROWS 2
#define H_COLS 4
#define R_ROWS 2
#define R_COLS 2
#define I_ROWS 4
#define I_COLS 4
#define P_ROWS 4
#define P_COLS 4
#define Y_ROWS 2
#define Y_COLS 1
#define K_ROWS 4
#define K_COLS 2
#define X_ROWS 4
#define X_COLS 1
#define Z_ROWS 2
#define Z_COLS 1
#define I_ROWS 4
#define I_COLS 4



struct Sensor {
	int z;
	int hitStart;
	int hitNums;
};

struct Hit {
	float x;
	float y;
};

struct Track { // 57 + 24*4 = 324 B
	float x0;
	float tx;
	float y0;
	float ty;

	float s0;
	float sx;
	float sz;
	float sxz;
	float sz2;

	float u0;
	float uy;
	float uz;
	float uyz;
	float uz2;
	
	char hitsNum;
	int hits[TRACK_SIZE];
};


#endif

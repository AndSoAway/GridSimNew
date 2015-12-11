#ifndef CONFIG_H
#define CONFIG_H 1
#include "point.h"
#include "../Grid/rectangle.h"

#define ENLARGE 10000

#define TIMEINTERVAL 90
#define MINPOINTSIZE 5
#define MAXPOINTSIZE 50

//distance unit 500m
//#define DISUNIT 500
#define DISUNIT 100
#define DMAX 500

#define BJMAXY 41.083332 
#define BJMAXX 117.5
#define BJMINY 39.416668
#define BJMINX 115.375

#define R 6367000

#define PI 3.14159265

#define TOP_PRECISE 177957.253042
#define BOTTOM_PRECISE 182394.780236

#define LEFT_PRECISE 185188.635477
#define RIGHT_PRECISE 185188.635477

//(TOP_PRECISE + BOTTOM_PRECISE) / 2
#define WIDTH 180176.016638
//(LEFT_PRECISE + RIGHT_PRECISE) / 2
#define HEIGHT 185188.635477

//Enlarge by ENLARGE times
#define BJYDIF 16666.64
#define BJXDIF 21250

// WIDTH / BJXDIF
#define LEN_PER_X  8.478871 
// HEIGHT / BJYDIF
#define LEN_PER_Y 11.111335

#define PRECISE 1e-5


//----trajectory.cc----
#define ANGLETHRESHOLD 135.0

const  Point left_bottom(BJMINX * ENLARGE, BJMINY * ENLARGE);  
const  Point right_upper(BJMAXX * ENLARGE, BJMAXY * ENLARGE);
const  Rectangle BJMAP(left_bottom, right_upper);
#endif

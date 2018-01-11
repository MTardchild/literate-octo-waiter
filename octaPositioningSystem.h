#ifndef OCTA_POSITIONING_SYSTEM_H
#define OCTA_POSITIONING_SYSTEM_H

#include "rpmSensor.h"

#define OPS_CALC_FREQUENCY 100

/*
 *	x position (square wise)
 */
byte ops_xSquare = 6;

/*
 *	y position (square wise)
 */
byte ops_ySquare = 1;

/*
 *	x position inside square (0 - 255)
 */
byte ops_x = 127;

/*
 *	y position inside square (0 - 255)
 */
byte ops_y = 127;

/*
 *	The direction waiter is facing in degrees (0-359)
 *  0 = North
 *  	The robot always starts facing south.
 */
float ops_dir = 180;

/*
 *	Cm equivalent of a square point.
 */
const float squarePointDistance = 0.09803922;

/*
 *	Cm equivalent of a wheel rotation.
 */
const float rotationDistance = 13.67;

/*
 *	Represents 360/Pi.
 */
const float turnConst = 114.591559026;

/*
 *	Distance between the middle of the wheels in cm.
 */
const float wheelBase = 10.2;

task calculatePosition(); 
void addDistance(int distanceX, int distanceY);
void dpmToDistance(float dpmA, float dpmB); 
int getDirectionSignX();
int getDirectionSignY();
float getRatioX();
void addDirection(float value);
void dpmToAngle(float dpmA, float dpmB);
float rpmToDpm(byte motor); 

/*
 *	Calculates the current position and writes result into global variables. 
 *	This method assumes that it gets called every OPS_CALC_FREQUENCY ms.
 */
task calculatePosition() {
	while(1) {
		float dpmA = rpmToDpm(OUT_A); 
		float dpmB = rpmToDpm(OUT_B);
		Wait(OPS_CALC_FREQUENCY);
		dpmToAngle(dpmA, dpmB);	
		dpmToDistance(dpmA, dpmB);
#ifdef DEBUG
		ClearLine(LCD_LINE1);
		ClearLine(LCD_LINE2);
		TextOut(50, LCD_LINE1, "X");
		NumOut(0, LCD_LINE1, ops_xSquare);
		NumOut(20, LCD_LINE1, ops_x);
		TextOut(50, LCD_LINE2, "Y");
		NumOut(0, LCD_LINE2, ops_ySquare);
		NumOut(20, LCD_LINE2, ops_y);
#endif
	}
}

/*
 *	Adds given distance to x and y coordinates.
 */
void addDistance(int distanceX, int distanceY) {
    int tempX = ops_x + distanceX;
    int tempY = ops_y + distanceY;

	if (tempX > 255) ++ops_xSquare;
    else if (tempX < 0) --ops_xSquare;

    if (tempY > 255) ++ops_ySquare;
    else if (tempY < 0) --ops_ySquare;

    ops_x += distanceX;
	ops_y += distanceY;
}

/*
 *	Calculates the new coordinates.
 */
void dpmToDistance(float dpmA, float dpmB) {
	float average = (dpmA + dpmB) / 2;
	float distance = average * OPS_CALC_FREQUENCY / 1000;
    int signX = getDirectionSignX();
    int signY = getDirectionSignY();
	float ratioX = getRatioX();
	float ratioY = 1 - ratioX;
	addDistance(average * ratioX * signX, average * ratioY * signY);
}

/*
 *  Returns whether the x component has to be decreased or increased.
 */
int getDirectionSignX() {
    return ops_dir > 0 && ops_dir < 180 ? 1 : -1;
}

/*
 *  Returns whether the y component has to be decreased or increased.
 */
int getDirectionSignY() {
    return ops_dir > 270 || ops_dir < 90 ? 1 : -1;
}

/*
 *	Calculates how much the robot drove into x direction.
 *	Relative to y.
 *	Examples:
 *	if ops_dir = 45 the ratio will be 0.5 for x and y.
 *	if ops_dir = 90 the ratio will be 1 for x and 0 for y.
 */
float getRatioX() {
	int ratio = ops_dir % 180;
	if (ratio > 90) ratio -= 90;
	float ratioX = ratio / 90;
	
	return ratioX;
}

/*
 *	Adds given value to direction variable.
 *	Takes care of overflow.
 */
void addDirection(float value) {
	ops_dir += value;
	ops_dir -= ops_dir > 359 ? 360 : 0; 	
}

/*
 *	Calculates the new coordinate given the dpm.
 */
void dpmToAngle(float dpmA, float dpmB) {
	float distanceAxisA = dpmA * OPS_CALC_FREQUENCY / 1000;
	float distanceAxisB = dpmB * OPS_CALC_FREQUENCY / 1000;
	float distanceDiff = distanceAxisA - distanceAxisB;
	float directionOffset = (distanceDiff/wheelBase) * turnConst;	
	addDirection(directionOffset);
}

/*
 *	Converts rpm into distance/minute
 */
float rpmToDpm(byte motor) {
	float distance;

	switch(motor) {
		case OUT_A:
			distance = rpmA;
		case OUT_B:
			distance = rpmB;
		case OUT_C:
			distance = rpmC;
	}

	return distance * rotationDistance * squarePointDistance; 
}

#endif // OCTA_POSITIONING_SYSTEM_H


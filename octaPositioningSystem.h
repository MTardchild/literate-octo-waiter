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
float ops_dir = 179;

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
 *	Distance between the middle of the wheels in square points.
 */
const float wheelBase = 9.3 / squarePointDistance;

task calculatePosition(); 
void addDistance(int distanceX, int distanceY);
void dpmToDistance(float dpmA, float dpmB); 
int getDirectionSignX();
int getDirectionSignY();
float getRatioY();
void addDirection(float value);
void dpmToAngle(float dpmA, float dpmB);
void dpmToAngleTurning(float dpmA, float dpmB);
float rpmToDpm(byte motor); 

#define sameSign(x, y) ((x < 0 && y < 0 || x >= 0 && y >= 0) ? 1 : 0)

/*
 *	Calculates the current position and writes result into global variables. 
 *	This method assumes that it gets called every OPS_CALC_FREQUENCY ms.
 */
task calculatePosition() {
	while(1) {
		Wait(OPS_CALC_FREQUENCY);
		float dpmA = rpmToDpm(OUT_A); 
		float dpmB = rpmToDpm(OUT_B);
		#ifdef DEBUG
			ClearLine(LCD_LINE1);
			TextOut(0, LCD_LINE1, "ops_x");
			NumOut(50, LCD_LINE1, ops_x);			

			ClearLine(LCD_LINE2);
			TextOut(0, LCD_LINE2, "ops_y");
			NumOut(50, LCD_LINE2, ops_y);			
			
			ClearLine(LCD_LINE6);
			TextOut(0, LCD_LINE6, "ops_dir");
			NumOut(50, LCD_LINE6, ops_dir);			
		
		#endif

		
		if (sameSign(dpmA, dpmB)){
			dpmToAngle(dpmA, dpmB);	
			dpmToDistance(dpmA, dpmB);
		}else{
			dpmToAngleTurning(dpmA, dpmB);
		}
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
	float distance = average * OPS_CALC_FREQUENCY / 60 / 1000;
    int signX = getDirectionSignX();
    int signY = getDirectionSignY();
	float ratioY = getRatioY();
	float ratioX = 1 - ratioY;
	

#ifdef DEBUG
	ClearLine(LCD_LINE3);
	TextOut(0, LCD_LINE3, "ratioX");
	NumOut(50, LCD_LINE3, ratioX);
#endif

	addDistance(distance * ratioX * signX, distance * ratioY * signY);
}

/*
 *  Returns whether the x component has to be decreased or increased.
 */
int getDirectionSignX() {
    return (ops_dir > 0 && ops_dir < 180) ? 1 : -1;
}

/*
 *  Returns whether the y component has to be decreased or increased.
 */
int getDirectionSignY() {
    return (ops_dir > 270 || ops_dir < 90) ? 1 : -1;
}

/*
 *	Calculates how much the robot drove into x direction.
 *	Relative to y.
 *	Examples:
 *	if ops_dir = 45 the ratio will be 0.5 for x and y.
 *	if ops_dir = 90 the ratio will be 1 for x and 0 for y.
 */
float getRatioY() {
	int ratio = ops_dir % 180;
	ratio -= 90;
	float ratioY = ratio / 90;
	
	return abs(ratioY);
}

/*
 *	Adds given value to direction variable.
 *	Takes care of overflow.
 */
void addDirection(float value) {
	ops_dir += value;
	if (ops_dir < 0)
		ops_dir += 360;
	else if(ops_dir > 359)
		ops_dir -= 360;
}

/*
 *	Calculates the new coordinate given the dpm.
 */
void dpmToAngle(float dpmA, float dpmB) {
	float distanceAxisA = dpmA * OPS_CALC_FREQUENCY / 1000 / 60;
	float distanceAxisB = dpmB * OPS_CALC_FREQUENCY / 1000 / 60;
	float distanceDiff = distanceAxisA - distanceAxisB;
	float directionOffset = (distanceDiff/(wheelBase*2)) * turnConst;	
	addDirection(directionOffset);
}

void dpmToAngleTurning(float dpmA, float dpmB) {
	float distanceAxisA = dpmA * OPS_CALC_FREQUENCY / 1000 / 60;
	float distanceAxisB = dpmB * OPS_CALC_FREQUENCY / 1000 / 60;
    float distance = (abs(distanceAxisA) + abs(distanceAxisB)) / 2;
	float angle = distance / wheelBase * turnConst;
	addDirection((dpmA > dpmB) ? angle : angle * (-1));
}	

/*
 *	Converts rpm into distance/minute
 */
float rpmToDpm(byte motor) {
	float rpm;

	switch(motor) {
		case OUT_A:
			rpm = rpmA;
			break;
		case OUT_B:
			rpm = rpmB;
			break;
		default:
			break;
	}
	
	return rpm * rotationDistance / squarePointDistance; 
}

#endif // OCTA_POSITIONING_SYSTEM_H


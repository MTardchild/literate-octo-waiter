#ifndef OCTA_ADJUSTMENT_CONTROLLER_H
#define OCTA_ADJUSTMENT_CONTROLLER_H

#define kp 0.6
#define ki 0.1
#define kd 0.0

#define T 0.3

#define SOLL 35
#define SOCKELWERT 20
#define pController(ist) ((SOLL-ist) * kp)
#define EPSILON_DIRECTION 0.5

/*
 *  Stores the smoothness passed as parameter to an interface function (0 - 255).
 *  This is needed because parameters can not be passed into tasks.
 */
byte smoothness = -1;

/*
 *  Stores the power used to control the engines passed as parameter to 
 *  an interface function (0 - 100). 
 *  This is needed because parameters can not be passed into tasks.
 */
byte power = -1;

/*
 *  true: Robot is actually in the process of turning.
 *  false: Robot is actually not turning.
 */
bool isTurning = false;

/*
 *  Stores the cause of the robot stopping.
 *
 *  0x01: irSensor
 */
byte stopCause = 0;

/*
 *  Stores the direction the robot is actually facing (0 - 359).
 *
 *  0: North // 45: North-East
 *  90: East // 135: South-East
 *  180: South // 225: South-West
 *  270: West // 315: North-West
 */
short actualDirection = -1;

/*
 *  Stores the target direction of the turning motion (0 - 359).
 *
 *  0: North // 45: North-East
 *  90: East // 135: South-East
 *  180: South // 225: South-West
 *  270: West // 315: North-West
 */
short targetDirection = -1;

/*
 *  Stores the already driven distance since engine turn on.
 */
int actualDistance = -1;

/*
 *  Stores the distance that has to be driven.
 */
int targetDistance = -1;

/*
 *  Infinite loop that checks if events have been set.
 *  Pulls data from globals in sensors.h.
 *  Calls corresponding on<Event> functions.
 */
task mo_checkEvents();

int iController(int ist)
{
  static int s = 0;
  s += (SOLL - ist);
  return ki * T * s;
}

int dController(int ist)
{
  static int delta_alt = 0;
  int a = kd * (SOLL-ist - delta_alt) / T;
  delta_alt = SOLL - ist;

  return a;
}

int piController(int ist)
{
  return pController(ist) + i_regler(ist);
}

int pdController(int ist)
{
  return pController(ist) + d_regler(ist);
}

int pidController(int ist)
{
  return pController(ist) + d_regler(ist) + i_regler(ist);
}

/*
 *  Drives in currently facing direction. Default power value.
 *
 *  smoothness: Determines how smooth the motion should be (0 - 255).
 *  distance: Determines the distance of the drive (in mm).
 */
void move(byte smoothness, unsigned int distance);

/*
 *  Drives in currently facing direction.
 *
 *  smoothness: Determines how smooth the motion should be (0 - 255).
 *  power: Determines the power (in %) used to control the engines (0 - 100).
 *  distance: Determines the distance of the drive (in mm).
 */
void movePower(byte smoothness, byte power, unsigned int distance); 

/*
 *  Turns the robot so that it faces the given direction. Default power value.
 *
 *  smoothness: Determines how smooth the motion should be (0 - 255).
 *  direction: Determines the direction the robot should face afterwards. (0 - 359)
 */
void turn(byte smoothness, unsigned short direction);

/*
 *  Turns the robot so that it faces the given direction.
 *
 *  direction: Determines the direction the robot should face afterwards. (0 - 359)
 *  power: Determines the power (in %) used to control the engines (0 - 100).
 *  smoothness: Determines how smooth the motion should be (0 - 255).
 */
void turnPower(byte smoothness, byte power, unsigned short direction);

void start_drive(){


}

void stop_drive(){


}

void drive(int dist){
	

}

void start_turn(int dir){


}

void stop_turn(int dir){


}

void turn(int dir){
	

}

bool isFacingDirection(short direction) {
    return actualDirection >= direction - EPSILON_DIRECTION
        && actualDirection <= direction + EPSILON_DIRECTION;
}

/*
 *  Returns: Distance (in mm) driven since the engine power on.
 */
int getDistanceDriven() {

}
	
void onDistanceDrivenChanged(short newDistance) {
    if (targetDistance <= newDistance) {
        // Stop
    } 

    actualDistance = newDistance;
}

void onDistanceIrChanged(short newDistance) {
    if (!isTurning) {
        if (newDistance <= DISTANCE_IR_THRESHOLD) {
            // stop
            stopCause = 0x01;
        }
    }
}

void onDirectionChanged(short newDirection) {
    if (isTurning) {
        if (isFacingDirection(newDirection)) {
            isTurning = false;
            // stop
        } 
    } else {
        if (actualDirection > newDirection + EPSILON_DIRECTION) {
            isTurning = true;
            // turn right
        } else if (actualDirection < newDirection - EPSILON_DIRECTION) {
            isTurning = true;
            // turn left
        }
    }

    actualDirection = newDirection;
}

/*
 *  Called when the compass sensor reports a new value.
 *
 *  newDirection: Direction the robot actually faces (0 - 359).
 */
void onDirectionChanged(short newDirection);

/*
 *  Called when the ir sensor reports a new value.
 *
 *  newDistance: Distance (in mm) of an obstacle the ir sensor 
 *  measured (500 - 8000).
 */
void onDistanceIrChanged(short newDistance);

/*
 *  Called when the rpm measurement and calculation reports a change
 *  in driven distance.
 *
 *  newDistance: Distance driven since the engines have been turned on (in mm).
 */
void onDistanceDrivenChanged(short newDistance);

task ac_checkEvents() {
    if (eventIsPresent(DistanceDrivenChanged)) {
        onDistanceDrivenChanged(se_distanceDriven);
    }
    
    if (eventIsPresent(DistanceIrChanged)) {
        onDistanceIrChanged(se_distanceIr);
    }

    if (eventIsPresent(DirectionChanged)) {
        onDirectionChanged(se_direction);
    }
}

task main() {
  long rotCount, oldRotCount;
  int power = 0;

  oldRotCount = MotorRotationCount(OUT_A);

  while (1) {
     Wait(T*1000);

     rotCount = MotorRotationCount(OUT_A);
     TextOut(0, LCD_LINE2, "    ");
     int rpm = (rotCount - oldRotCount) / T * 60 / 360;
     NumOut(0, LCD_LINE2, rpm);
     power += piController(rpm);
     //OnFwd(OUT_A, power);
     OnFwd(OUT_A, SOCKELWERT + pidController(rpm));

     oldRotCount = rotCount;
  }
}

#endif // OCTA_ADJUSTMENT_CONTROLLER_H

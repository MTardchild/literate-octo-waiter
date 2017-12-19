#ifndef OCTA_MOVER_H
#define OCTA_MOVER_H

#define EPSILON_DIRECTION 0.5

/*
 *  Stores the current step the robot is in (0 - MAX_PATH_SIZE).
 *  This essentially keeps track of pf_path[].
 */
byte currentStep = -1;

/*
 *  true: Robot is currently in the process of turning.
 *  false: Robot is currently not turning.
 */
bool isTurning = false;

/*
 *  Stores the cause of the robot stopping.
 *
 *  0x01: irSensor
 */
byte stopCause = 0;

/*
 *  Stores the direction the robot is currently facing (0 - 359).
 *
 *  0: North // 45: North-East
 *  90: East // 135: South-East
 *  180: South // 225: South-West
 *  270: West // 315: North-West
 */
short currentDirection = -1;

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
int currentDistance = -1;

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

/* 
 *  Moves the path calculated by pathfinder.
 *  Default smoothness and power values.
 */
void movePath();

/*
 *  Called when the compass sensor reports a new value.
 *
 *  newDirection: Direction the robot currently faces (0 - 359).
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

task mo_checkEvents() {
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

void movePath() {

}

void onDistanceDrivenChanged(short newDistance) {
    if (targetDistance <= newDistance) {
        // Stop
    } 

    currentDistance = newDistance;
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
        if (newDirection >= targetDirection + EPSILON_DIRECTION
            || newDirection <= targetDirection - EPSILON_DIRECTION) {
            isTurning = false;
            // stop
        } 
    } else {
        if (currentDirection > newDirection + EPSILON_DIRECTION) {
            isTurning = true;
            // turn right
        } else if (currentDirection < newDirection - EPSILON_DIRECTION) {
            isTurning = true;
            // turn left
        }
    }

    currentDirection = newDirection;
}

#endif // OCTA_MOVER_H

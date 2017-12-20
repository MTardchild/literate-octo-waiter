#ifndef OCTA_MOVER_H
#define OCTA_MOVER_H

/*
 *  Stores the actual step the robot is in (0 - MAX_PATH_SIZE).
 *  This essentially keeps track of pf_path[].
 */
byte currentStep = -1;

/* 
 *  Moves the path calculated by pathfinder.
 *  Default smoothness and power values.
 */
void movePath();

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



void movePath() {
    while (currentStep != MAX_PATH_SIZE) {
        consecutiveSameDirections = 1;
        if (isFacingDirection(pf_path[currentStep]) {
            ++currentStep;
            while(isFacingDirection(pf_path[currentStep]) {
                ++consecutiveSameDirections;
                ++currentStep;
            }

            // move consecutiveSameDirections squares forward
        } else {
            // turn to target direction
        }
    }
}

#endif // OCTA_MOVER_H

#ifndef OCTA_SENSORS_H
#define OCTA_SENSORS_H

#define COMPASS_SENSOR_PORT S2

/*
 *  Infinite loop checking the sensors.
 *  Throws corresponding events.
 */
task checkSensors();

/*
 *  Last calculated distance of ir sensor measurement.
 */
int se_distanceIr = -1;

/*
 *  Last calculated distance driven since engine power on.
 */
int se_distanceDriven = -1;

/*
 *  Last measured direction.
 */
short se_direction = -1;

task checkSensors() {
    while(true) {
        Wait(100);
        if (distanceIr != getDistanceIr()) {
            distanceIr = getDistanceIr();
            setEvent(DistanceIrChanged);
        } 
        
        if (distanceDriven != getDistanceDriven()) {
            distanceDriven = getDistanceDriven();
            setEvent(DistanceDrivenChanged);
        } 
        
        if (direction != getCompassDirection()) {
            direction = getCompassDirection();
            setEvent(DirectionChanged);
        }
    }
}

#endif // OCTA_SENSORS_H

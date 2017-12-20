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
int se_rpm = -1;

/*
 *  Last measured direction.
 */
short se_direction = -1;

task checkSensors() {
    while(true) {
        Wait(100);
        if (distanceIr != getDistanceIr()) {
			se_distanceIr = getDistanceIr();
        } 
        
        if (distanceDriven != getDistanceDriven()) {
            se_rpm_a = getRpm(OUT_A);
			se_rpm_b = getRpm(OUT_B);
			se_rpm_c = getRpm(OUT_C);
        } 
        
        if (direction != getCompassDirection()) {
            se_direction = getCompassDirection();
        }
    }
}

#endif // OCTA_SENSORS_H

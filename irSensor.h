#ifndef OCTA_IR_SENSOR_H
#define OCTA_IR_SENSOR_H

#define IR_SENSOR_PORT S1

/*
 *  Returns: Distance (in mm) of next obstacle in LoS (500 - 8000).
 */
int getDistanceIr() {
	return calculateDistance(SensorRaw(IR_SENSOR_PORT));
}

/*
 *  Maps the raw value to mm.
 *
 *  val: Raw sensor output.
 *  Returns: Distance to obstacle in mm.
 */
int calculateDistance(int val){
    int sens[9] = {780, 560, 400, 320, 280, 260, 240, 220, 210};
    int dist[9] = {5, 10, 20, 30, 40, 50, 60, 70, 80};

    int i = 0;
    while(sens[i] > val && i < 8) i++;
    
    if(i == 0)
         return dist[i] * 100;

    int sens_diff = sens[i-1] - sens[i];
    int dist_diff = dist[i] - dist[i-1];
    
    val -= sens[i];
    
    return dist[i]*100 - (dist_diff*100 / sens_diff * val);
}

#endif // OCTA_IR_SENSOR_H

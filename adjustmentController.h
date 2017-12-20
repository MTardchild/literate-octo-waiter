#ifndef OCTA_ADJUSTMENT_CONTROLLER_H
#define OCTA_ADJUSTMENT_CONTROLLER_H

#define kp 0.0006
#define ki 0.0001
#define kd 0.0001


/*
* pController	
* target: target rpm
* current: current rpm
* returns the new power value
*/
#define pController(target, current) ((target-current) * kp)

/*
* 
* out: the engine number	
* target: target rpm
* current: current rpm
* T: time since last call
* returns the new power value
*/


int iControllerA(int target, int current, int s)
{
  static int s = 0;
  s += (target - current);
  return ki * T * s;
}

int dControllerA(int target, int current, int T)
{
  static int old_delta = 0;
  int a = kd * (target-current - old_delta) / T;
  old_delta = target - current;
  return a;
}

int iControllerB(int target, int current, int s)
{
  static int s = 0;
  s += (target - current);
  return ki * T * s;
}

int dControllerB(int target, int current, int T)
{
  static int old_delta = 0;
  int a = kd * (target-current - old_delta) / T;
  old_delta = target - current;
  return a;
}

int iControllerC(int target, int current, int s)
{
  static int s = 0;
  s += (target - current);
  return ki * T * s;
}

int dControllerC(int target, int current, int T)
{
  static int old_delta = 0;
  int a = kd * (target-current - old_delta) / T;
  old_delta = target - current;
  return a;
}

inline int piController(int target, int current, int T)
{
  return pController(current, T) + iController(current, T);
}

inline int pdController(int target, int current, int T)
{
  return pController(target, current, T) + dController(target, current, T);
}

inline int pidController(int target, int current, int T)
{
  return pController(target, current) + dController(target, current, T) + iController(target, current, T);
}

#endif // OCTA_ADJUSTMENT_CONTROLLER_H


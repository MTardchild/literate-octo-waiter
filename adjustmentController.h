#ifndef OCTA_ADJUSTMENT_CONTROLLER_H
#define OCTA_ADJUSTMENT_CONTROLLER_H

// parameters constants to adjust smoothness
#define kp 0.015
#define ki 0.001
#define kd 0.0025


// public prototypes

/*
* Implements various controller types
* out: engine number
* target: target rpm
* current: current rpm
* returns the new power value, for that engine
*/
#define pController(target, current) ((target-current) * kp)
int piController(int out, float target, float current);
int pdController(int out, float target, float current);
int pidController(int out, float target, float current);

void ac_resetAll();


// crazy macros starting here

/*
* Utility function.
* Required set up for getTimeSinceLastCall().
*/
#define setUpLocalTimer() \
  static unsigned long ticks = 0;                                           \
  if (!ticks) ticks = CurrentTick();                                        \
  unsigned long last_called = ticks;                                        \
  ticks = CurrentTick();                                                    \

/*
* Must be used after a call to setUpLocalTimer!
* Returns the time since the last call to the function this is called in
* in milliseconds.
* Returns 0 in the 1st call
*/
#define getTimeSinceLastCall() (ticks-last_called)

/*
* Macro to avoid copy pasta for different controller types
*/
#define controllerMacro(x)  \
int x##Controller(int out, float target, float current){                        \
    switch (out){                                                           \
    case OUT_A:                                                             \
        x##Macro(A);                                                        \
    case OUT_B:                                                             \
        x##Macro(B);                                                        \
    case OUT_C:                                                             \
        x##Macro(C);                                                        \
    }                                                                       \
}

/*
* Macro to avoid copy pasta for different engines
* Used to create one iController function per engine
*/
#define iMacro(x)  \
int iController##x(float target, float current, bool do_reset=false)        \
{                                                                           \
  static float sum = 0;     												\
  if (do_reset)																\
	  sum=0;  														\
  setUpLocalTimer();                                                        \
  sum += (target - current);                                                \
  return ki * getTimeSinceLastCall() * sum;                                 \
}

/*
* Build iControllerx functions, where x is one of [A, B, C]
*/
iMacro(A)
iMacro(B)
iMacro(C)
#undef iMacro

/*
* Macro to avoid copy pasta for different engines
* Used to create one dController function per engine
*/
#define dMacro(x)  \
int dController##x(float target, float current, bool do_reset=false)        \
{                                                                           \
  static float old_delta = 0;                                               \
  if (do_reset)																\
	  old_delta=0;															\
  setUpLocalTimer();                                                        \
  int a = kd * (target-current - old_delta) / getTimeSinceLastCall();       \
  old_delta = target - current;                                             \
  return a;                                                                 \
}

/*
* Build dControllerx functions, where x is one of [A, B, C]
*/
dMacro(A)
dMacro(B)
dMacro(C)
#undef dMacro

/*
* Build xController functions, where x is one of [pid, pi, pd]
*/
#define piMacro(x) \
    return pController(target, current) + dController##x(target, current)
controllerMacro(pi)
#undef piMacro

#define pdMacro(x) \
    return pController(target, current) + dController##x(target, current)
controllerMacro(pd)
#undef pdMacro

#define pidMacro(x) \
    return pController(target, current) + iController##x(target, current) + dController##x(target, current)
controllerMacro(pid)
#undef pidMacro

#undef controllerMacro
#undef setUpLocalTimer
#undef getTimeSinceLastCall


void ac_resetAll(){
	iControllerA(0, 0, 1);
	dControllerA(0, 0, 1);
	iControllerB(0, 0, 1);
	dControllerB(0, 0, 1);
	iControllerC(0, 0, 1);
	dControllerC(0, 0, 1);
}

#endif // OCTA_ADJUSTMENT_CONTROLLER_H


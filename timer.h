#ifndef OCTA_TIMER_H
#define OCTA_TIMER_H

#define timerCount 5
#define TimerType unsigned int

typedef struct{
      unsigned int time;
      unsigned long startTime;
      EventType ev;
      bool isActive;
}Timer;

Timer timers[timerCount];

void initTimers(){
     int i;
     for(i=0; i< timerCount; i++)
           timers[i].isActive = false;
}

void setTimer(TimerType id, unsigned int time, EventType ev){
     timers[id].time = time;
     timers[id].ev = ev;
}

void changeTimer(TimerType id, unsigned int time) {
     timers[id].time = time;
}

void startTimer(TimerType id) {
     timers[id].startTime = CurrentTick();
     timers[id].isActive = true;
}

void cancelTimer(TimerType id) {
     timers[id].isActive = false;
}

task checkTimers() {
    unsigned short i;
    while(true){
         Wait(100);
		 NumOut(0, LCD_LINE2, CurrentTick() - timers[0].startTime);
         for(i=0; i < timerCount; i++){
             if(timers[i].isActive
                   && CurrentTick() - timers[i].startTime > timers[i].time){
                    timers[i].isActive = false;
                    setEvent(timers[i].ev);
              }
         }
    }
}

#endif // OCTA_TIMER_H

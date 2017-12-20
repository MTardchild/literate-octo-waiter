#ifndef OCTA_EVENTS_H
#define OCTA_EVENTS_H

#define DistanceDrivenChanged 0x01
#define DistanceIrChanged 0x02
#define DirectionChanged 0x04

#define EventType unsigned int
EventType eTracker = 0;
void setEvent(EventType ev){eTracker |= ev;}
#define eventIsPresent(ev) (ev & eTracker)
void clearEvent(EventType ev) {eTracker &= ~(ev);}

#endif // OCTA_EVENTS_H

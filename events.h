#ifndef OCTA_EVENTS_H
#define OCTA_EVENTS_H


#define EventType unsigned int
EventType eTracker = 0;
void setEvent(EventType ev){eTracker |= ev;}
#define eventIsPresent(ev) (ev & eTracker)
void clearEvent(EventType ev) {eTracker &= ~(ev);}

#endif // OCTA_EVENTS_H

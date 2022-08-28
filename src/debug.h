#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define d(x) Serial.print(x)
#define dln(x) Serial.println(x)
#else
#define d(x)
#define dln(x)
#endif

#endif


#include <Arduboy2.h> 
#include <ArduboyTones.h>

#ifndef ARDUBOY_UTIL_ALE_H
#define ARDUBOY_UTIL_ALE_H

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);


#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}



#endif /* ARDUBOY_UTIL_ALE_H */


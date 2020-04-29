#include "MKL25Z4.h"

/*----------------------------------------------------------------------------
 * Constants and variables for all the LED pins
 *---------------------------------------------------------------------------*/
#define GC1 7 // PortC Pin 7
#define GC2 0 // PortC Pin 0
#define GC3 3 // PortC Pin 3
#define GC4 4 // PortC Pin 4
#define GC5 5 // PortC Pin 5
#define GC6 6 // PortC Pin 6
#define GC7 10 // PortC Pin 10
#define GC8 11 // PortC Pin 11

#define RED 21 // PortE Pin 21

int green[10] = {MASK(GC1), MASK(GC2), MASK(GC3), MASK(GC4), MASK(GC5),
                                            MASK(GC6), MASK(GC7), MASK(GC8)};
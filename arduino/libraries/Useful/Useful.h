//
// Useful constants and routines
//
// Copyright Tarim - January 2011
//
// Useful is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Useful is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Useful.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __ARDUINO_USEFUL__
#define __ARDUINO_USEFUL__

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#define NULL			((void)0)
#define length( x )		( sizeof(x) / sizeof(*(x)) )
#define incmod( val, modulo )	( (val) = (val) == (modulo)-1 ? 0 : (val)+1 )
#define decmod( val, modulo )	( (val) = (val) == 0 ? (modulo)-1 : (val)-1 )
#define bit( x )		(1ul << (x))
#define quote_( x ) #x
#define quote( x ) quote_( x )

#ifdef DEBUG
    #define assert(assertion, label) typedef int label[(assertion) ? 0 : -1]
#else
    #define assert(assertion, label)
#endif

#ifdef DEBUG
extern unsigned int __data_start;
extern unsigned int __data_end;
extern unsigned int __bss_start;
extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
int free_memory;
  
    if((int)__brkval == 0)
	free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
	free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
};

    #define printFreeMemory() { \
	Serial.print( freeMemory() ); \
	Serial.print( "/" ); \
	Serial.println( RAMEND ); \
    }

#else
    #define printFreeMemory()
#endif



#endif

Waiter
======

  A template class for timing things without using delay().



Introduction
============

  The Arduino millis() function returns a 4 byte value in milliseconds - which will time any duration up to about 49 days to the nearest millisecond.  Most timings don't require this level of precision.  For example, timings of around a minute can usually be done to the nearest second - which will fit in a single byte.

  Waiter provides a template class for easy timing control at arbitrary precision and resolution.

  Waiter can also use the micros() function (or any other timer function).

- Precision from milliseconds to hours
- Duration resolution can be 1, 2 or 4 byte value
- Works correctly when any resolution clock wraps around



WAITER Declarations
===================

  The following declarations are provided:

  The full 4 byte timer to millisecond precision:

        typedef WAITER<int32_t, 0> Waiter;

  A 2 byte timer to the nearest second:

        typedef WAITER<int16_t, 10> WaitSec;

  To use the existing second timer, all you need is:

        WaitSec timer;



WAITER Methods
==============


Waiter.reset()
--------------

  Reset the timer to _now_.  e.g.

        Waiter timer;
        timer.reset();


Waiter.alarm( _time_ )  
----------------------

  True if more than _time_ has passed.  _time_ is measured in the units of the timer (e.g. seconds for WaitSec, milliseconds for Waiter.)

        Waiter timer;
        timer.reset();
        unsigned long last = millis();
        while( 1 ) {
            if( timer.alarm( timer.seconds( 2 ) ) ) {
                timer.reset();
                unsigned long now = millis();
                Serial.println( now - last );
                last = now;
            }
        }

  Outputs 2000 every 2 seconds.


Waiter.set( _time_ )  
--------------------

  Set the alarm to go off at _time_.  The equivalent of the previous program:

        Waiter timer;
        timer.set( timer.seconds( 2 ) );
        unsigned long last = millis();
        while( 1 ) {
            if( timer.alarm( 0 ) ) {
                timer.set( timer.seconds( 2 ) );
                unsigned long now = millis();
                Serial.println( now - last );
                last = now;
            }
        }


Waiter.wait( _time_ )  
---------------------

  True if more than _time_ has passed and sets the timer to go off in _time_.  The equivalent of timer.reset( _time_ ) when timer.alarm( 0 ) is true.

        Waiter timer;
        timer.reset();
        unsigned long last = millis();
        while( 1 ) {
            if( timer.wait( timer.seconds( 2 ) ) ) {
                unsigned long now = millis();
                Serial.println( now - last );
                last = now;
            }
        }


Waiter.repeat( _time_ )  
-----------------------

  True if more than _time_ has passed and adds _time_ on to the timer.  This is subtly different to Waiter.wait in that it can "catch up" when not checked continually.  For example:

        Waiter timer;
        timer.reset();
        unsigned long last = millis();
        while( 1 ) {
            if( timer.repeat( timer.seconds( 2 ) ) ) {
                unsigned long now = millis();
                Serial.println( now - last );
                last = now;
                delay( random( 0, 3000 ) );
            }
        }

  May delay less than 2000ms, but will always average to 2000ms.  Replacing timer.repeat with timer.wait will always give delays of at least 2000ms.


Waiter.ms( _milliseconds_ )
---------------------------

  Returns _milliseconds_ in the units of the timer, rounded up.


Waiter.seconds( _seconds_ )  
---------------------------

  Returns _seconds_ in the units of the timer (e.g. 1 for WaitSec, 1000 for Waiter).  As a timer's precision is always in powers of 2 milliseconds - this can sometimes give surprising but, in fact, more accurate results.

        WaitSec timer;
        Serial.println( timer.seconds( 45 ) );

  Gives the result of 44.  WaitSec is actually measuring periods of 1.024 seconds, and 44*1.024 = 45.056, which is closer to the desired 45 second delay.


Waiter.minutes( _minutes_ )
---------------------------

  Returns _minutes_ in the units of the timer.


Waiter.hours( _hours_ )
-----------------------

  Returns _hours_ in the units of the timer.


Waiter.days( _days_ )
---------------------

  Returns _days_ in the units of the timer.


Waiter.TIME_MAX
---------------

  Returns the maximum duration the timer can time in the units of the timer.



WAITER Template
===============

  The template is declared:

        template <class ALARM_T = int32_t, uint8_t SHIFT_ = 0, clock_t(*TIMER)(void) = millis>
        class WAITER

  ALARM_T is must be a *signed* type and is the resolution of the timer.  The timer will take up this much space in RAM.  SHIFT is the number of bits millis() is right shifted by.  To give some examples:

  A millisecond timer with 4 byte storage (the full range of millis()) which can time up to 2^31ms (~24 days):

        typedef WAITER<int32_t, 0> Waiter;

  A millisecond timer with 2 byte storage which can time up to 2^15 milliseconds (~32 seconds):

        typedef WAITER<int16_t, 0> WaitmS;

  The same but can only time up to 2^7 milliseconds, (~1/8 second):

        typedef WAITER<int8_t, 0> WaitmSTiny;

  2 byte timer with a precision of 2^10 milliseconds (1.024 seconds) and a resolution of up to 2^15 seconds (~9 hours).  It is predefined as the WaitSec timer.  Note that this does not time exact numbers of seconds.  If that accuracy is important - use a millisecond timer.

        typedef WAITER<int16_t, 10> WaitSec;

  1 byte timer with a precision of 2^7 milliseconds (~1/8 second) and a resolution of 2^14 milliseconds (~16 seconds).  Might be useful for the rate of a flashing LED:

        typedef WAITER<int8_t, 7> WaitFlash;

  2 byte timer with a precision of 2^16 milliseconds (~1 minute) which can time up to 2^15 minutes (~24 days):

        typedef WAITER<int16_t, 16> WaitMinute;

  1 byte timer with a precision of 2^16 milliseconds (~1 minute) which can time up to 2^7 minutes (~2 hours):

        typedef WAITER<int8_t, 16> WaitMinuteTiny;

  1 byte timer with a precision of 2^24 milliseconds (~4 hours) which can time up to 2^7 x 4 hours (~24 days):

        typedef WAITER<int8_t, 24> Wait4Hour;

  Do not specify more than 32 bits in total.  Using a shift of 28 would fail as an attempt to create a 3 day timer even with a resolution of int8_t as 28+8 is greater than 32.  The 4 hour timer (shift 24) is the least precision timer.



About WAITER
============

Copywrite Tarim 2015

Distributed under the GNU Lesser General Public License


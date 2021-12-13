//
// Waiter class
//
// Copyright Tarim - 2011, 2014, 2015, 2016
//
// Waiter is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Waiter is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Waiter.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __ARDUINO_WAITER__
#define __ARDUINO_WAITER__

#if defined(ARDUINO)
    #if ARDUINO >= 100
        #include "Arduino.h"
    #else
        #include "WProgram.h"
    #endif
#endif

typedef unsigned long clock_t;

//
// WAITER class template
//   ALARM_T            class of alarm time
//   SHIFT              bit shift of time()
//   TIMER              TIMER ? micros() : millis()
//
// WAITER.set           set alarm duration from now
// WAITER.reset         reset alarm to now
// WAITER.alarm         true when alarm time has passed
// WAITER.wait          alarm() and reset()
// WAITER.repeat        alarm() and add to alarm time
//
// WAITER.seconds       seconds in template units
// WAITER.minutes       minutes in template units
// WAITER.hours         hours in template units
// WAITER.days          days in template units
//

template <class ALARM_T = int32_t, uint8_t SHIFT_ = 0, clock_t(*TIMER)(void) = millis>
class WAITER {
public:
    //
    // alarm_t          internal alarm type
    // TIME_MAX         maximum alarm time for this template
    // alarmTime        internal alarm time
    //
    typedef ALARM_T alarm_t;
    static const uint8_t ALARM_SHIFT = sizeof(alarm_t) * 8 - 1;

    // this should give a warning if SHIFT_ + ALARM_SHIFT >= 32
    // provided we use OVERFLOW somewhere
    // if only Arduino compiler warning levels were set :(
    static const int32_t OVERFLOW = bit( SHIFT_ + ALARM_SHIFT );
    static const uint8_t SHIFT = SHIFT_ + 0*OVERFLOW;

    static const alarm_t TIME_MAX = bit( ALARM_SHIFT ) - 1;

    alarm_t alarmTime;

    WAITER() {
        alarmTime = 0;
    };



    //
    // difference between shifted now and time returned as type alarm_t
    //
    inline alarm_t difference( const clock_t now, const alarm_t time ) {
        return (now >> SHIFT) - time;
    };

    //
    // compare difference
    //
    inline bool compare( const clock_t now, const alarm_t time ) {
        return difference( now, time ) >= 0;
    };

    //
    // adjust time to internal alarm_t with rounding up
    //
    inline alarm_t adjust( const clock_t time ) {
        return (time + bit(SHIFT)-1) >> SHIFT;
    };



    //
    // set the next alarm time
    //
    inline void set( const alarm_t duration ) {
        alarmTime = adjust( TIMER() ) + duration;
    };

    //
    // reset next alarm time to now
    //
    inline void reset() {
        set( 0 );
    };



    //
    // alarm is true if alarmTime has passed
    //
    inline bool alarm( const alarm_t duration = 0 ) {
        return compare( TIMER(), alarmTime + duration );
    };

    //
    // wait until alarmTime has passed, then set next alarmTime
    //
    bool wait( const alarm_t duration ) {
        const clock_t now = TIMER();
        if( compare( now, alarmTime + duration ) ) {
            alarmTime = adjust( now );
            return true;
        }
        return false;
    };

    //
    // repeat: wait until alarmTime has passed and add duration to alarmTime
    //
    bool repeat( const alarm_t duration ) {
        if( compare( TIMER(), alarmTime + duration ) ) {
            alarmTime += duration;
            return true;
        }
        return false;
    };



    //
    // ms: number of milliseconds in template units
    //
    inline alarm_t ms( const clock_t time ) {
        return adjust( time );
    };

    //
    // seconds: number of seconds in template units
    //
    inline alarm_t seconds( const clock_t time ) {
        return adjust( time * 1000ul );
    };

    //
    // minutes: number of minutes in template units
    //
    inline alarm_t minutes( const clock_t time ) {
        return adjust( time * 1000ul * 60ul );
    };

    //
    // hours: number of hours in template units
    //
    inline alarm_t hours( const clock_t time ) {
        return adjust( time * 1000ul * 60ul * 60ul );
    };

    //
    // days: number of days in template units
    //
    inline alarm_t days( const clock_t time ) {
        return adjust( time * 1000ul * 60ul * 60ul * 24ul );
    };
};

// millisecond timer, maximum time ~24 days
typedef WAITER<> Waiter;

// ~second (1.024 second) timer, maximum time ~9.5 hours
typedef WAITER<int16_t, 10> WaitSec;


// millisecond timer, maximum time ~30 seconds
// typedef WAITER<int16_t> WaitmS;

// ~minute (65.536 second) timer, maximum time ~24 days
// typedef WAITER<int16_t, 16> WaitMinute;


// millisecond timer, maximum time ~quarter-second
// typedef WAITER<int8_t> WaitmSTiny;

// ~eighth-second (0.128 second) timer, maximum time ~15 seconds
// typedef WAITER<int8_t, 7> WaitEighthSecTiny;

// ~quarter-second (0.256 second) timer, maximum time ~30 seconds
// typedef WAITER<int8_t, 8> WaitQuarterSecTiny;

// ~second (1.024 second) timer, maximum time ~2 minutes
// typedef WAITER<int8_t, 10> WaitSecTiny;

// ~minute (65.536 second) timer, maximum time ~2 hours
// typedef WAITER<int8_t, 16> WaitMinuteTiny;

// ~4 hour (4.660 hour) timer, maximum time ~24 days
// typedef WAITER<int8_t, 24> Wait4HourTiny;

// ~3 day (3.1 day) timer, impossible as 28 + size of alarm_t > size of clock_t
// typedef WAITER<int8_t, 28> 0impossible;

#endif

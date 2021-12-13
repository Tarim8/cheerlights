//
// Enttec simulation
//
// Copyright Tarim - 2017
//
// Enttec simulation is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Enttec simulation is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Enttec simulation.  If not, see <http://www.gnu.org/licenses/>.

// Requires:
// #include <Streaming.h>
// #include <Waiter.h>

// For DMX shield, requires:
// #include <DmxSimple.h>

//
// Enttec Class template takes:
//
// A DMXClass which provides:
//   .use_pin( int channels )           set pin for dmx shield
//   .maxChannel( int channels )        set the number of output channels
//   .write( int channel, char ch )     set the channel value
// 
// A ConstsClass which provides enttec packet framing constants and pin numbers
//
template <class DmxClass, class ConstsClass>
class EnttecClass {
public:
    enum { START, COMMAND, LOWCHAN, HIGHCHAN, DATA, FINISH };
    byte state;
    unsigned char ch;

    unsigned int readMaxChannels;
    unsigned int maxChannels;
    unsigned int count;

    Waiter timer;
    DmxClass dmx;
    ConstsClass consts;

    //
    // Initialise the DMX Shield if it exists
    //
    void dmxInit() {
        pinMode( consts.dmxEnable, OUTPUT );
        digitalWrite( consts.dmxEnable, HIGH );
        dmx.usePin( consts.dmxOutput );

        pinMode( consts.ledPin, OUTPUT );
        digitalWrite( consts.ledPin, HIGH );
        delay( 500 );

        reset();
    };

    //
    // Reset the Enttec variables
    //
    void reset() {
        maxChannels = 0;
        dmx.maxChannel( 0 );
        timer.reset();
        digitalWrite( consts.ledPin, LOW );
    };

    //
    // Error is usually called for some kind of Enttec packet framing error
    //
    virtual void error( const char message ) {
        reset();
    };

    //
    // Got a valid Enttec packet
    //
    virtual void ok() {
        digitalWrite( consts.ledPin, HIGH );
    };

    //
    // Check latest character is what it should be
    //
    bool matches( const unsigned char match ) {
        if( ch == match ) {
            ++state;
            return true;

        } else {
            error( 'M' );
            state = START;
            return false;
        }
    };

    //
    // Do the work
    //
    void read( const int ch_ ) {
        if( ch_ < consts.charBase ) {
            return;
        }
        ch = ch_ - consts.charBase;

        // If we've been idle and get a start of packet then resync
        if( state != START && ch == consts.enttecStart && timer.alarm( consts.idleTime ) ) {
            error( 'R' );
            state = START;
        }
        timer.reset();

        switch( state ) {
        case START:
            matches( consts.enttecStart );
            break;

        case COMMAND:
            matches( consts.enttecSend );
            break;

        case LOWCHAN:
            readMaxChannels = ch;
            ++state;
            break;

        case HIGHCHAN:
            readMaxChannels += ch << 8;
            if( maxChannels != readMaxChannels ) {
                if( readMaxChannels <= consts.absMaxChannels ) {
                    maxChannels = readMaxChannels;
                    dmx.maxChannel( maxChannels );
                    if( maxChannels == 0 ) {
                        error( '0' );
                        break;
                    }

                } else {
                    error( 'C' );
                    break;
                }
            }

            count = 0;
            ++state;
            break;

        case DATA:
            if( count < maxChannels ) {
                dmx.write( count, ch );
                ++count;
                break;
            }

            ++state;
            // continue

        case FINISH:
            if( matches( consts.enttecFinish ) ) {
                ok();
            }
            state = START;
            break;
        }
    };
};



//
// Standard Enttec constants
//
class EnttecConsts {
public:
    static const unsigned char charBase         = 0;
    static const unsigned char enttecStart      = '\x7e';
    static const unsigned char enttecSend       = '\x06';
    static const unsigned char enttecFinish     = '\xe7';
    static const clock_t idleTime               = 200;
    static const unsigned int absMaxChannels    = 512;
    static const unsigned int dmxEnable         = 0;
    static const unsigned int dmxOutput         = 0;
    static const unsigned int ledPin            = 0;
};

//
// Use as a base class when there is no DMX shield
//
class DmxDummyClass {
public:
    void usePin( const unsigned int pin ) {};

    void maxChannel( const unsigned int channels ) {};

    void write( const unsigned int channel, unsigned int ch ) {};
};





//
// Allow for testing from a keyboard
//
class ManualConsts: public EnttecConsts {
public:
    static const unsigned char charBase     = ' ';
    static const unsigned char enttecStart  = 'S'-' ';
    static const unsigned char enttecSend   = 'C'-' ';
    static const unsigned char enttecFinish = 'F'-' ';
};

//
// Report DMX calls for debugging
//
template <int displayChannels>
class DmxTestClass {
public:
    void usePin( const unsigned int pin ) {
        Serial << F("usePin ") << pin << endl;
    };

    void maxChannel( const unsigned int channels ) {
        Serial << F("maxChannels ") << channels << endl;
    };

    void write( const unsigned int channel, unsigned int ch ) {
        if( channel <= displayChannels ) {
            Serial << channel << ':' << ch << endl;
        }
    };
};

// Use this to debug a real Enttec interface
// class EnttecTest: public EnttecClass<DmxTestClass<10>, EnttecConsts> {

class EnttecTest: public EnttecClass<DmxTestClass<512>, ManualConsts> {
public:
    void error( const char message ) {
        Serial << message << F(" state=") << (int)state
               << F(" char=") << (unsigned int)ch
               << F(" chans=") << readMaxChannels
               << endl;
        reset();
    };
};





//
// Only create a real DMX Shield if DmxDimple.h is included
//
#ifdef DmxSimple_h
    class SKPangConsts: public EnttecConsts {
    public:
        // SK Pang DMX rev E and rev F boards
        static const unsigned int dmxOutput         = 2;
        static const unsigned int dmxEnable         = 4;
        static const unsigned int ledPin            = 8;
    };

    typedef EnttecClass<DmxSimpleClass, SKPangConsts> EnttecSKPang;
#endif



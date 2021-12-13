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

#define VERSION "0.91"

#include <Streaming.h>
#include <Waiter.h>
#include <Useful.h>
#include <DmxSimple.h>
#include "enttectest.h"

// Fixed values
const long int BAUD = 115200;



class ExampleConsts: public EnttecConsts {
public:
    static const unsigned int absMaxChannels = 500;
    static const unsigned int ledPin = 0;
};

class ExampleClass: public DmxDummyClass {
public:
    void write( const unsigned int channel, unsigned int ch ) {
        if( channel <= 64 ) {
            Serial << channel << ':' << ch << endl;
        }
    };
};

typedef EnttecClass<ExampleClass, ExampleConsts> EnttecExample;

EnttecSKPang enttec;
//EnttecTest enttec;
//EnttecExample enttec;


//
// usual suspects
//
void setup() {
    delay( 500 );
    Serial.begin( BAUD );

    enttec.dmxInit();

    Serial << VERSION << endl;
    printFreeMemory();
};

void loop() {
    enttec.read( Serial.read() );
};

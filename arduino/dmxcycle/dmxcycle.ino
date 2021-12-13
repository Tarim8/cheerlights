#include <DmxSimple.h>
#include <Streaming.h>

//
// DMXcycle lighting installation
//
// Copyright Tarim - 2021
//
// DMXcycle is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// DMXcycle is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with DMXcycle.  If not, see <http://www.gnu.org/licenses/>.

#define DEBUG

const unsigned int CHANNEL = 4;
const unsigned int CHANNELS_LENGTH = 3;

typedef byte pin_t;
const long int BAUD = 9600;

// SK Pang DMX rev E and rev F boards
const byte skpang_dmx_output = 2;
const byte skpang_dmx_enable = 4;

const char VERSION[] =
    #include "version.h"
;

// LED and sensor pins
const byte shieldLED = 8;
const byte arduinoLED = 13;
const byte SW1 = A1;

#include <Useful.h>



//
// Color class - write a color to the dmx controller
//
class Color {
public:
    char* name;
    byte intensity[CHANNELS_LENGTH];

    void set() {
        Serial << name;

        for (byte j = 0; j < CHANNELS_LENGTH; ++j) {
            Serial << ' ' << _HEX(intensity[j]);
            DmxSimple.write (CHANNEL + j, intensity[j]);
        }

        Serial << endl;
    };
} colors[] =
{
    { "red",     { 0xFF, 0x00, 0x00 } },
    { "green",   { 0x00, 0x80, 0x00 } },
    { "blue",    { 0x00, 0x00, 0xFF } },
    { "cyan",    { 0x00, 0xFF, 0xFF } },
    { "white",   { 0xFF, 0xFF, 0xFF } },
    { "oldlace", { 0xFD, 0xF5, 0xE6 } },
    { "purple",  { 0x80, 0x00, 0x80 } },
    { "magenta", { 0xFF, 0x00, 0xFF } },
    { "yellow",  { 0xFF, 0xFF, 0x00 } },
    { "orange",  { 0xFF, 0xA5, 0x00 } },
    { "pink",    { 0xFF, 0xC0, 0xCB } }
};



class Watcher {
public:
    bool state = 0;
    unsigned int color = 0;

    void poll() {
        const bool sw = digitalRead(SW1);
        if (state == sw) return;
        state = sw;
        if (state) return;

        incmod(color, length(colors));
        colors[color].set();
        delay(200);
    };
} watch;



//
// usual suspects
//
void setup() {
    pinMode( shieldLED, OUTPUT );
    pinMode( arduinoLED, OUTPUT );

    pinMode( SW1, INPUT_PULLUP );

    DmxSimple.usePin( skpang_dmx_output );
    pinMode( skpang_dmx_enable, OUTPUT );
    digitalWrite( skpang_dmx_enable, HIGH );

    digitalWrite( arduinoLED, HIGH );
    Serial.begin( BAUD );

    DmxSimple.maxChannel( CHANNEL + CHANNELS_LENGTH + 1 );

    delay( 1000 );
    Serial << VERSION << endl;
    printFreeMemory();

    digitalWrite( arduinoLED, LOW );
};

void loop() {
    watch.poll();
};

